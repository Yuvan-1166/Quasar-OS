; Kernel entry point assembly code
; This file provides the initial kernel entry point and stack setup

[BITS 32]           ; 32-bit protected mode
[SECTION .text]     ; Code section

; Declare external kernel_main function (from kernel.c)
extern kernel_main

; Global symbols that can be called from outside
global _start       ; Entry point for linker

; Kernel entry point
_start:
    ; Set up stack pointer
    mov esp, kernel_stack_top
    
    ; Clear direction flag (for string operations)
    cld
    
    ; Call the main kernel function
    call kernel_main
    
    ; If kernel_main returns, hang the system
hang:
    cli             ; Disable interrupts
    hlt             ; Halt the processor
    jmp hang        ; Infinite loop

; Reserve space for kernel stack (4KB)
[SECTION .bss]
resb 4096          ; 4KB stack
kernel_stack_top: