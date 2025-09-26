; Quasar-OS Interrupt Assembly Stubs
; Low-level interrupt service routines and IRQ handlers

[BITS 32]
[SECTION .text]

; Import C functions
extern interrupt_handler
extern irq_handler

; Export assembly functions
global idt_flush

; Macro for ISR stubs without error code
%macro ISR_NOERRCODE 1
    global isr%1
    isr%1:
        cli                 ; Disable interrupts
        push byte 0         ; Push dummy error code
        push byte %1        ; Push interrupt number
        jmp isr_common_stub ; Jump to common handler
%endmacro

; Macro for ISR stubs with error code
%macro ISR_ERRCODE 1
    global isr%1
    isr%1:
        cli                 ; Disable interrupts
        push byte %1        ; Push interrupt number
        jmp isr_common_stub ; Jump to common handler
%endmacro

; Macro for IRQ stubs
%macro IRQ 2
    global irq%1
    irq%1:
        cli                 ; Disable interrupts
        push byte 0         ; Push dummy error code
        push byte %2        ; Push IRQ number
        jmp irq_common_stub ; Jump to common handler
%endmacro

; Define ISR stubs (0-31 are CPU exceptions)
ISR_NOERRCODE 0     ; Division By Zero Exception
ISR_NOERRCODE 1     ; Debug Exception
ISR_NOERRCODE 2     ; Non Maskable Interrupt Exception
ISR_NOERRCODE 3     ; Int 3 Exception
ISR_NOERRCODE 4     ; INTO Exception
ISR_NOERRCODE 5     ; Out of Bounds Exception
ISR_NOERRCODE 6     ; Invalid Opcode Exception
ISR_NOERRCODE 7     ; Coprocessor Not Available Exception
ISR_ERRCODE   8     ; Double Fault Exception (With Error Code!)
ISR_NOERRCODE 9     ; Coprocessor Segment Overrun Exception
ISR_ERRCODE   10    ; Bad TSS Exception (With Error Code!)
ISR_ERRCODE   11    ; Segment Not Present Exception (With Error Code!)
ISR_ERRCODE   12    ; Stack Fault Exception (With Error Code!)
ISR_ERRCODE   13    ; General Protection Fault Exception (With Error Code!)
ISR_ERRCODE   14    ; Page Fault Exception (With Error Code!)
ISR_NOERRCODE 15    ; Reserved Exception
ISR_NOERRCODE 16    ; Floating Point Exception
ISR_ERRCODE   17    ; Alignment Check Exception
ISR_NOERRCODE 18    ; Machine Check Exception
ISR_NOERRCODE 19    ; Reserved

; Define IRQ stubs (32-47 are hardware interrupts)
IRQ   0,    32      ; Timer
IRQ   1,    33      ; Keyboard
IRQ   2,    34      ; Cascade (used internally by PICs)
IRQ   3,    35      ; COM2
IRQ   4,    36      ; COM1
IRQ   5,    37      ; LPT2
IRQ   6,    38      ; Floppy Disk
IRQ   7,    39      ; LPT1
IRQ   8,    40      ; CMOS real-time clock
IRQ   9,    41      ; Free for peripherals / legacy SCSI / NIC
IRQ  10,    42      ; Free for peripherals / SCSI / NIC
IRQ  11,    43      ; Free for peripherals / SCSI / NIC
IRQ  12,    44      ; PS2 Mouse
IRQ  13,    45      ; FPU / Coprocessor / Inter-processor
IRQ  14,    46      ; Primary ATA Hard Disk
IRQ  15,    47      ; Secondary ATA Hard Disk

; Common ISR stub - saves processor state, calls C handler, restores state
isr_common_stub:
    pusha               ; Push edi,esi,ebp,esp,ebx,edx,ecx,eax
    
    mov ax, ds          ; Lower 16-bits of eax = ds
    push eax            ; Save the data segment descriptor
    
    mov ax, 0x10        ; Load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    call interrupt_handler
    
    pop ebx             ; Reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    
    popa                ; Pop edi,esi,ebp,esp,ebx,edx,ecx,eax
    add esp, 8          ; Clean up the pushed error code and ISR number
    sti                 ; Re-enable interrupts
    iret                ; Pop CS, EIP, EFLAGS, SS, and ESP

; Common IRQ stub
irq_common_stub:
    pusha               ; Push edi,esi,ebp,esp,ebx,edx,ecx,eax
    
    mov ax, ds          ; Lower 16-bits of eax = ds
    push eax            ; Save the data segment descriptor
    
    mov ax, 0x10        ; Load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    call irq_handler
    
    pop ebx             ; Reload the original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    
    popa                ; Pop edi,esi,ebp,esp,ebx,edx,ecx,eax
    add esp, 8          ; Clean up the pushed error code and IRQ number
    sti                 ; Re-enable interrupts
    iret                ; Pop CS, EIP, EFLAGS, SS, and ESP

; Load the IDT
idt_flush:
    mov eax, [esp+4]    ; Get the pointer to the IDT, passed as a parameter
    lidt [eax]          ; Load the IDT pointer
    ret