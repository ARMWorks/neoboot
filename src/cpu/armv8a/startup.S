.text

    .globl  _reset
_reset:
    b       _start


    .org    _reset + 96, 0
_start:
    /* Check for CPU #0 */
    mrs     x0, mpidr_el1
    and     x0, x0, #3
    cmp     x0, #0
    bne     hang

//    ldr     x1, =vectors
//    msr     vbar_el1, x1

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