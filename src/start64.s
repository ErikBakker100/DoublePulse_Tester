#include "..\lib\general\include\sysregs.h"

.section .text._start, "ax"
.extern core_main_0
.extern core_main_1
.extern core_main_2
.extern core_main_3

.globl _start
.globl core_entry_1
.globl core_entry_2
.globl core_entry_3
.extern irq_handler_0
.extern irq_handler_1
.extern irq_handler_2
.extern irq_handler_3
.extern __stack_el1_top_core0
.extern __stack_el1_top_core1
.extern __stack_el1_top_core2
.extern __stack_el1_top_core3

// -----------------------------------------------------
//  Entry point voor Core0
// -----------------------------------------------------
_start:

    // - x0 contains the physical address of the device tree blob (DTB) in RAM, set by the bootcode/bootloader.
    // - Normally we will start in EL2 (Exception Level 2, hypervisor mode) with SP_EL2 as stack pointer.
    // Clear the BSS segment (assumes __bss_start and __bss_end word-aligned).
    ldr     x6, =__bss_start        // Current word of the BSS (initially at start).
    ldr     x7, =__bss_end          // Address one past the BSS segment.
	sub	    x7, x7, x6
bss_clear_loop:
	str     xzr, [x6], #8
	subs    x7, x7, #8
	b.gt    bss_clear_loop          // b.ge (Greater or Equal) springt als x0 >= x1 (signed). b.gt (Greater Than) springt als x0 > x1 (signed).

// -----------------------------------------------------
// Entry voor Core0–3
// -----------------------------------------------------
.macro	core_entry_x	core
core_entry_\core:
    // Determine current exception level
    mrs     x1, CurrentEL           // Put the current exception level in x1.
    lsr     x1, x1, #2              // CurrentEL >> 2 gives EL number (0..3)
    // Als we al in EL1 zijn, ga dan direct naar el1_entry_core0
    cmp     x1, #1                  // Compare current EL with 1
    beq     el1_entry_core\core     // If we are in EL1 nothing more to do, jump to el1_entry_core0

    // We are in EL2: set exception vector base for EL2 and prepare stack pointers and exception vector base for EL1,

    // Prepare SPSR_EL2 to return to EL1, using EL1h and with interrupts masked.
    // Chosen SPSR value encodes: AArch64, EL1h, DAIF = all masked (A/I/F/D). M[3:0] = 0b0101 = 0x5  → EL1h mode
    // Put CPU in a known state
	ldr	    x4, =SCTLR_VALUE_MMU_DISABLED
	msr	    sctlr_el1, x4		
    isb
    mov     x6, (1 << 31)           // Hypervisor configuration: aarch64 mode for EL1.
    msr     hcr_el2, x6             // Written to the HCR_EL2 system register.
    
    mov     x6, #0x3c5              // 0x3C5 = 0b 11 1100 0101 -> DAIF = all masked (A/I/F/D), M[3:0] = 0b0101 = 0x5  → EL1h mode
    msr     spsr_el2, x6            // SPSR (Saved Program Status Register) Bewaard het proces wanneer een EL2 exception optreed. 
                                    // bits [9,8,7,6] → D, A, I, F bits (Debug, SError, IRQ, FIQ) interrupt masks. 1 = masked, 0 = unmasked.
                                    // bit [4] → AArch64 execution state (0 = AArch64, 1 = AArch32)
                                    // bits [3:0] → mode field 0b00101 = 0x5 → EL1h mode (EL1 using SP_EL1)
                                    // Dus bij terugkeer naar EL1 zijn alle interrupts gemaskeerd.
                                    // Zie ARM DDI 0487A.a, hoofdstuk B3.5.1 "Saved Program Status Register (SPSR_EL2)".
                                    // Wanneer een ERET wordt uitvoert vanuit EL2: SPSR_EL2 → bepaalt in welke mode EL1 start (EL1h/EL1t, DAIF, flags)
    // Set ELR_EL2 to the address we'll return to in EL1
    ldr     x6, =el1_entry_core\core
    msr     elr_el2, x6
    // Now return to EL1 (execution continues at el1_entry_core0 in EL1)
    eret

el1_entry_core\core:
    // We are now executing in EL1.    
    mrs     x2, CurrentEL           // Put the current exception level in x2.
    lsr     x2, x2, #2              // CurrentEL gives EL number (0..3)
    // Set up stack pointer for EL1
    ldr x6, =__stack_el1_top_core\core
    mov sp, x6
    // Install an exception vector.
    ldr x6, =vector_table_\core
    msr vbar_el1, x6
    isb
    bl      core_main_\core
// If main returns, park the core.
    b       hang
.endm

core_entry_x 0
core_entry_x 1
core_entry_x 2

// ---------------------------------------------------------
// AArch64 Vector Tables (EL1) voor core 0-3
// ---------------------------------------------------------
// Vector table macro
// ---------------------------------------------------------
	.macro	ventry	label
	.align	7
	b	\label
	.endm

.macro vector_table_x core
    .section .vectors_core\core, "ax"
    .align 11                       // 2048 bytes zoals ARM vereist
vector_table_\core:
// --- EL1t ---
    ventry hang
    ventry hang
    ventry hang
    ventry hang
// --- EL1h (THIS IS WHAT WE USE) ---
    ventry core_entry_\core
    ventry irq_entry_\core
    ventry hang
    ventry hang
// --- From EL0 ---
    ventry hang
    ventry hang
    ventry hang
    ventry hang
// --- unused entries ---
    ventry hang
    ventry hang
    ventry hang
    ventry hang
.endm

vector_table_x 0
vector_table_x 1
vector_table_x 2

// -----------------------------------------------------
// IRQ handlers for cores 0–2
// -----------------------------------------------------
// Macro to save registers and call the C IRQ handler
.macro	kernel_entry
.section .text
.align 4
    // Reserve enough bytes (31 regs * 8 = 248 bytes; + 8 padding => 256 = 16*16)
    sub     sp, sp, #(16*16)          // ruimte voor x0–x30, aligned op 16 bytes
    stp     x0,  x1,  [sp, #16*0]
    stp     x2,  x3,  [sp, #16*1]
    stp     x4,  x5,  [sp, #16*2]
    stp     x6,  x7,  [sp, #16*3]
    stp     x8,  x9,  [sp, #16*4]
    stp     x10, x11, [sp, #16*5]
    stp     x12, x13, [sp, #16*6]
    stp     x14, x15, [sp, #16*7]
	stp	    x16, x17, [sp, #16*8]
	stp	    x18, x19, [sp, #16*9]
	stp	    x20, x21, [sp, #16*10]
	stp	    x22, x23, [sp, #16*11]
	stp	    x24, x25, [sp, #16*12]
	stp	    x26, x27, [sp, #16*13]
	stp	    x28, x29, [sp, #16*14]
    stp     x30, xzr, [sp, #16*15]   // bewaar x30 (link register)
	.endm

// Macro to restore registers and return from irq
.macro	kernel_exit
.section .text
.align 4
    ldp     x0,  x1,  [sp, #16*0]
    ldp     x2,  x3,  [sp, #16*1]
    ldp     x4,  x5,  [sp, #16*2]
    ldp     x6,  x7,  [sp, #16*3]
    ldp     x8,  x9,  [sp, #16*4]
    ldp     x10, x11, [sp, #16*5]
    ldp     x12, x13, [sp, #16*6]
    ldp     x14, x15, [sp, #16*7]
	ldp	    x16, x17, [sp, #16*8]
	ldp	    x18, x19, [sp, #16*9]
	ldp	    x20, x21, [sp, #16*10]
	ldp	    x22, x23, [sp, #16*11]
	ldp	    x24, x25, [sp, #16*12]
	ldp	    x26, x27, [sp, #16*13]
	ldp	    x28, x29, [sp, #16*14]
    ldp     x30, xzr, [sp, #16*15]   // laad x30 (link register)
    add     sp, sp, #(16*16)
    eret
    .endm

.section .text
irq_entry_0:
    kernel_entry
    bl      irq_handler_core0
    kernel_exit

irq_entry_1:
    kernel_entry
    bl      irq_handler_core1
    kernel_exit

irq_entry_2:
    kernel_entry
    bl      irq_handler_core2
    kernel_exit

// -----------------------------------------------------

hang:
    wfe                             // Wait For Event: zet de core in een laag-energie wacht-toestand totdat een interrupt of event optreedt.
    b hang
