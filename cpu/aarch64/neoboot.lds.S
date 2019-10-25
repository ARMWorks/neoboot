#define SRAM_A1 (0x00010000)
#define SRAM_C (0x00018000)

OUTPUT_FORMAT("elf64-littleaarch64", "elf64-bigaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)
SECTIONS
{
    . = SRAM_A1;
    .text : { *(.text) }
    .data : { *(.data) }
    bss_start = .;
    .bss : { *(.bss) }
    bss_end = .;
    . = ALIGN(16);
    . = . + 0x1000; /* 4k stack */
    stack_top = .;
}