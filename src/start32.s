.section .text._start, "ax"
.globl _start
.include "macros.S"

// -----------------------------------------------------
//  Start Core0
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
// Jump to core 0 entry point
    b       core_entry_0

// -----------------------------------------------------
//  Entry Core 0-1
// -----------------------------------------------------
core_entry_x 0
core_entry_x 1
// -----------------------------------------------------
//  Entry Irq's Core 0-1
// -----------------------------------------------------
irq_entry_x	0
irq_entry_x	1
// -----------------------------------------------------
//  Vector tables Core 0-1
// -----------------------------------------------------
vector_core_x 0
vector_core_x 1
// -----------------------------------------------------
hang:
    b hang