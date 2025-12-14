#ifndef MACROS_S
#define MACROS_S

.macro	    core_entry_x	core
.globl core_entry_\core
.extern core_main_\core
.extern __stack_svc_top_core\core
.extern __stack_irq_top_core\core
.extern __stack_fiq_top_core\core

core_entry_\core:
    cpsid if                     // disable IRQ and FIQ
    // set vector table
    ldr     r5, =vector_table_core\core
    mcr     p15, 0, r5, c12, c0, 0
    isb
    // Check if we are in Hyp mode
    mrs     r1, cpsr
    and     r1, r1, #0x1F
    cmp     r1, #0x1A           // If in hypervisor mode switch to SVC mode
    bne     svc_entry_core\core
    ldr     r3, =svc_entry_core\core
    msr     ELR_hyp, r3
    mrs     r4, cpsr
    bic     r4, r4, #0x1F
    orr     r4, r4, #0x13        // SPSR_hyp = SVC mode (mode bits only)
    msr     SPSR_hyp, r4
    eret
svc_entry_core\core:
    // setup SVC stack
    ldr     sp, =__stack_svc_top_core\core
    // setup IRQ stack
    mov     r5, #0xD2           // IRQ mode, disable FIQ and IRQ        
    msr     cpsr_c, r5
    ldr     sp, =__stack_irq_top_core\core
    // setup FIQ stack
    mov     r5, #0xD3           // FIQ mode, disable FIQ and IRQ        
    msr     cpsr_c, r5
    ldr     sp, =__stack_fiq_top_core\core
    // back to SVC
    mov     r5, #0xD3           // SVC mode, disable FIQ and IRQ
    msr     cpsr_c, r5
    ldr     sp, =__stack_svc_top_core\core
    bl      core_main_\core
    b       hang
.endm

.macro	    irq_entry_x	core
.globl irq_entry_\core
irq_entry_\core:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    bl irq_handler_core\core
    ldmfd sp!, {r0-r12, pc}^
.endm

.macro	    vector_core_x	core
.section .vectors_core\core, "ax"
.align 5

vector_table_core\core:
    b   core_entry_\core        // reset
    b   hang                    // undefined
    b   hang                    // swi
    b   hang                    // prefetch
    b   hang                    // data
    b   hang                    // unused
    b   irq_entry_\core         // irq
    b   hang                    // fiq
.endm

#endif // MACROS_S
