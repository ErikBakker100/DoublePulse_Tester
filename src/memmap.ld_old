
ENTRY(_start)

MEMORY
{
    ram : ORIGIN = 0x8000, LENGTH = 0x20000
/*    ram        (rwx) : ORIGIN = 0x8000,    LENGTH = 0x3F8000  /* RAM vanaf 0x8000 t/m ~4MB */
    peripherals(rwx) : ORIGIN = 0x20000000, LENGTH = 0x01000000 /* 16MB MMIO-peripherals BCM2835/6 */
}

SECTIONS
{
    .text : {
        *(.text._start)   /* _start komt hier terecht */
        *(.text*)
        *(.rodata*)
    } > ram

    .data : {
        *(.data*)
    } > ram

    .bss : {
        *(.bss COMMON)
    } > ram

    .stack (NOLOAD) : {
        . = ALIGN(8);
        __stack_top__ = .;
        . = . + 0x1000; /* 4KB stack */
        __stack_bottom__ = .;
    } > ram
}