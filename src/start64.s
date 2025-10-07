.section .text._start, "ax"
.global _start
_start:
    ldr x0, =__stack_top__
    mov sp, x0
    ldr x0, =main
    blr x0

hang:
    b hang
