    .text
    .globl  _start
_start:
	tst     x0, x0      // this is "b #0x84" in ARM
	b       reset
	.space  0x7c
	.word	0xe59f1024	// ldr     r1, [pc, #36] ; 0x170000a0
	.word	0xe59f0024	// ldr     r0, [pc, #36] ; CONFIG_*_TEXT_BASE
	.word	0xe5810000	// str     r0, [r1]
	.word	0xf57ff04f	// dsb     sy
	.word	0xf57ff06f	// isb     sy
	.word	0xee1c0f50	// mrc     15, 0, r0, cr12, cr0, {2} ; RMR
	.word	0xe3800003	// orr     r0, r0, #3
	.word	0xee0c0f50	// mcr     15, 0, r0, cr12, cr0, {2} ; RMR
	.word	0xf57ff06f	// isb     sy
	.word	0xe320f003	// wfi
	.word	0xeafffffd	// b       @wfi
	.word	0x09010040	// writeable RVBAR mapping address

#define COUNTER_FREQUENCY 24000000

/*
 * Branch according to exception level
 */
.macro	switch_el, xreg, el3_label, el2_label, el1_label
	mrs	\xreg, CurrentEL
	cmp	\xreg, 0xc
	b.eq	\el3_label
	cmp	\xreg, 0x8
	b.eq	\el2_label
	cmp	\xreg, 0x4
	b.eq	\el1_label
.endm

/* No-op */
.macro	set_vbar, regname, reg
.endm

reset:
	switch_el x1, 3f, 2f, 1f
3:	set_vbar vbar_el3, x0
	mrs     x0, scr_el3
	orr     x0, x0, #0xf			/* SCR_EL3.NS|IRQ|FIQ|EA */
	msr     scr_el3, x0
	msr     cptr_el3, xzr			/* Enable FP/SIMD */
	ldr     x0, = COUNTER_FREQUENCY
	msr     cntfrq_el0, x0			/* Initialize CNTFRQ */
	b       0f
2:	set_vbar vbar_el2, x0
	mov     x0, #0x33ff
	msr     cptr_el2, x0			/* Enable FP/SIMD */
	b       0f
1:	set_vbar vbar_el1, x0
	mov     x0, #3 << 20
	msr     cpacr_el1, x0			/* Enable FP/SIMD */
0:

    /* Hang if not CPU #0 */
    mrs     x0, mpidr_el1
    and     x0, x0, #3
    cmp     x0, #0
    bne     hang

    /* Configure stack */
    adrp    x0, stack_top
    mov     sp, x0

    /* Clear bss */
    ldr     x0, bss_start
    ldr     x1, bss_end
    mov     x2, #0
1:  stp     x2, x2, [x0, #16]!
    cmp     sp, x1
    ble     1b

    /* Jump to main */
    bl      main

hang:
    wfi
    b       hang