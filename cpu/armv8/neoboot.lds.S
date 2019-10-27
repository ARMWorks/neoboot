#define SRAM_A1 (0x00010000)
#define STACK_SIZE (4 << 10)

OUTPUT_FORMAT("elf64-littleaarch64", "elf64-bigaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_reset)
SECTIONS
{
    . = SRAM_A1;
    .text : { *(.text) }
    .data : { *(.data) }
    . = ALIGN(16);
    bss_start = .;
    .bss : { *(.bss) }
    . = ALIGN(16);
    bss_end = .;
    . = . + STACK_SIZE;
    stack_top = .;
}