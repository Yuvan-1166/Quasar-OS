; Quasar-OS Enhanced Bootloader
; Stage 1: Load kernel and switch to 32-bit protected mode

[BITS 16]
[ORG 0x7c00]

; Memory layout:
; 0x7c00 - 0x7dff: Bootloader (512 bytes)
; 0x8000 - 0x9fff: Kernel loading area (8KB)

KERNEL_LOAD_ADDR equ 0x8000     ; Where to load kernel in memory
KERNEL_SECTORS equ 16           ; Number of sectors to load (8KB)

start:
    ; Initialize segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00      ; Stack grows downward from bootloader

    ; Clear screen and display boot message
    call clear_screen
    mov si, boot_msg
    call print_string

    ; Load kernel from disk
    call load_kernel
    
    ; Check if kernel loaded successfully
    cmp byte [kernel_loaded], 1
    jne kernel_error

    mov si, kernel_loaded_msg
    call print_string

    ; Enable A20 line (required for >1MB memory access)
    call enable_a20

    ; Switch to 32-bit protected mode
    call switch_to_pm

kernel_error:
    mov si, kernel_error_msg
    call print_string
    jmp hang

; Load kernel from disk sectors 2-17 (after bootloader)
load_kernel:
    mov si, loading_msg
    call print_string

    ; Reset disk system
    mov ah, 0x00
    mov dl, 0x80        ; First hard drive
    int 0x13
    jc .disk_error

    ; Read kernel sectors
    mov ah, 0x02        ; Read sectors function
    mov al, KERNEL_SECTORS ; Number of sectors to read
    mov ch, 0x00        ; Cylinder 0
    mov cl, 0x02        ; Start from sector 2 (after bootloader)
    mov dh, 0x00        ; Head 0
    mov dl, 0x80        ; Drive 0 (first hard drive)
    mov bx, KERNEL_LOAD_ADDR ; Buffer to load into
    int 0x13            ; BIOS disk interrupt
    jc .disk_error

    ; Mark kernel as loaded
    mov byte [kernel_loaded], 1
    ret

.disk_error:
    mov si, disk_error_msg
    call print_string
    jmp hang

; Enable A20 line for >1MB memory access
enable_a20:
    ; Try keyboard controller method
    call wait_8042
    mov al, 0xd1
    out 0x64, al        ; Command: write to output port
    
    call wait_8042
    mov al, 0xdf
    out 0x60, al        ; Enable A20
    
    call wait_8042
    ret

wait_8042:
    in al, 0x64
    test al, 2
    jnz wait_8042
    ret

; Switch to 32-bit protected mode
switch_to_pm:
    cli                 ; Disable interrupts
    
    ; Load Global Descriptor Table
    lgdt [gdt_descriptor]
    
    ; Set PE bit in CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; Far jump to flush prefetch queue and enter 32-bit mode
    jmp 0x08:init_pm

; Print string function (16-bit mode)
print_string:
    mov ah, 0x0e
.loop:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .loop
.done:
    ret

; Clear screen function
clear_screen:
    mov ah, 0x00
    mov al, 0x03        ; 80x25 color text mode
    int 0x10
    
    ; Set cursor to top-left
    mov ah, 0x02
    mov bh, 0x00
    mov dh, 0x00
    mov dl, 0x00
    int 0x10
    ret

; Hang function
hang:
    hlt
    jmp hang

; 32-bit protected mode initialization
[BITS 32]
init_pm:
    ; Update segment registers for protected mode
    mov ax, 0x10        ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x7c00     ; Set stack pointer
    
    ; Jump to kernel
    call KERNEL_LOAD_ADDR
    
    ; If kernel returns, hang
    jmp $

; Global Descriptor Table
gdt_start:
    ; Null descriptor
    dq 0x0
    
    ; Code segment descriptor
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0000       ; Base (bits 0-15)
    db 0x00         ; Base (bits 16-23)
    db 10011010b    ; Access byte: present, ring 0, executable, readable
    db 11001111b    ; Granularity: 4KB blocks, 32-bit
    db 0x00         ; Base (bits 24-31)
    
    ; Data segment descriptor
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0000       ; Base (bits 0-15)
    db 0x00         ; Base (bits 16-23)
    db 10010010b    ; Access byte: present, ring 0, data, writable
    db 11001111b    ; Granularity: 4KB blocks, 32-bit
    db 0x00         ; Base (bits 24-31)
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; GDT size
    dd gdt_start                ; GDT address

; Data section
boot_msg           db 'Quasar-OS Bootloader v2.0', 0x0d, 0x0a, 0
loading_msg        db 'Loading kernel...', 0x0d, 0x0a, 0
kernel_loaded_msg  db 'Kernel loaded successfully!', 0x0d, 0x0a, 0
kernel_error_msg   db 'ERROR: Failed to load kernel!', 0x0d, 0x0a, 0
disk_error_msg     db 'ERROR: Disk read failed!', 0x0d, 0x0a, 0
kernel_loaded      db 0

; Pad to 510 bytes and add boot signature
times 510-($-$$) db 0
dw 0xAA55