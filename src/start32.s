.section .text._start, "ax"
.extern core_main_0
.extern core_main_1
.extern irq_handler_0
.extern irq_handler_1
.extern __stack_svc_top_core0
.extern __stack_irq_top_core0
.extern __stack_fiq_top_core0
.extern __stack_svc_top_core1
.extern __stack_irq_top_core1
.extern __stack_fiq_top_core1
.globl _start
.globl core_entry_1
.globl core_entry_2

// -----------------------------------------------------
//  Entry point voor Core0-1
// -----------------------------------------------------
_start:
    // Clear the BSS segment (assumes __bss_start and __bss_end word-aligned).
    ldr     r6, =__bss_start    // Current word of the BSS (initially at start).
    ldr     r7, =__bss_end      // Address one past the BSS segment.
    mov     r8, #0              // zero register
bss_clear_loop:
    cmp     r6, r7              // If we have reached the end ...
    bge     bss_clear_done      // ... exit the loop.
    str     r8, [r6]            // Otherwise, zero-out the word at address r6,
    add     r6, r6, #4          // increment r6 by 4 bytes,
    b       bss_clear_loop      // and continue to loop.
bss_clear_done:    
.macro	    core_entry_x	core
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

core_entry_x 0
core_entry_x 1

// -----------------------------------------------------
//  Entry Irq's Core 0-1
// -----------------------------------------------------
.macro	    irq_entry_x	core
irq_entry_\core:
    sub lr, lr, #4
    stmfd sp!, {r0-r12, lr}
    bl irq_handler_core\core
    ldmfd sp!, {r0-r12, pc}^
.endm

irq_entry_x	0
irq_entry_x	1
// -----------------------------------------------------
//  Vector tables Core 0-1
// -----------------------------------------------------
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

vector_core_x 0
vector_core_x 1
// -----------------------------------------------------
hang:
    b hang