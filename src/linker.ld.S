ENTRY(_start)

SECTIONS
{
    . = @LINKER_LOAD_ADDR@;

    /* --------------------------------------------------
     * Text
     * -------------------------------------------------- */
    .text :
    {
        *(.text._start)      /* _start altijd eerst */
        *(.text*)
    }

    /* --------------------------------------------------
     * Data
     * -------------------------------------------------- */
    .data :
    {
        *(.data*)
    }

    /* --------------------------------------------------
     * BSS
     * BSS segment (for uninitialised C global variables).
     * BSS stands for "block starting symbol".
     * The BSS segment must be zeroed prior to entering C code.
     * -------------------------------------------------- */
    .bss :
    {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }

    /* --------------------------------------------------
     * Exception vectors
     * -------------------------------------------------- */
    .vectors_core0 : ALIGN(0x800)
    {
        *(.vectors_core0)
    }

#ifdef DUALCORE
    .vectors_core1 : ALIGN(0x800)
    {
        *(.vectors_core1)
    }
#endif

    /* --------------------------------------------------
     * Stacks
     * -------------------------------------------------- */
    STACK_SIZE = 0x1000;   /* 4 KB per mode */
    . = ALIGN(16);

    __stack_start__ = .;

    /* ---------- Core 0 ---------- */
    __stack_svc_start_core0 = .;  . += STACK_SIZE;
    __stack_svc_top_core0   = .;

    __stack_irq_start_core0 = .;  . += STACK_SIZE;
    __stack_irq_top_core0   = .;

    __stack_fiq_start_core0 = .;  . += STACK_SIZE;
    __stack_fiq_top_core0   = .;

#ifdef DUALCORE
    /* ---------- Core 1 ---------- */
    __stack_svc_start_core1 = .;  . += STACK_SIZE;
    __stack_svc_top_core1   = .;

    __stack_irq_start_core1 = .;  . += STACK_SIZE;
    __stack_irq_top_core1   = .;

    __stack_fiq_start_core1 = .;  . += STACK_SIZE;
    __stack_fiq_top_core1   = .;
#endif

    /* --------------------------------------------------
     * Aliases / compatibility
     * -------------------------------------------------- */
    __stack_top__          = __stack_svc_top_core0;
    __stack_el1_top_core0  = __stack_svc_top_core0;

#ifdef DUALCORE
    __stack_el1_top_core1  = __stack_svc_top_core1;
#endif

    __stack_end__ = .;
}