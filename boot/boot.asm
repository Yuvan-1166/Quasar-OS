; Quasar-OS Bootloader
; A simple 16-bit bootloader that displays a welcome message

[BITS 16]           ; We start in 16-bit real mode
[ORG 0x7c00]        ; BIOS loads bootloader at 0x7c00

start:
    ; Clear the screen
    mov ah, 0x00    ; Set video mode function
    mov al, 0x03    ; 80x25 color text mode
    int 0x10        ; BIOS video interrupt

    ; Set cursor position to top-left
    mov ah, 0x02    ; Set cursor position
    mov bh, 0x00    ; Page number
    mov dh, 0x00    ; Row
    mov dl, 0x00    ; Column
    int 0x10

    ; Display welcome message
    mov si, welcome_msg
    call print_string

    ; Display version info
    mov si, version_msg
    call print_string

    ; Display instructions
    mov si, instruction_msg
    call print_string

    ; Infinite loop (halt system)
hang:
    hlt             ; Halt the CPU
    jmp hang        ; Loop forever

; Function to print a null-terminated string
; Input: SI = pointer to string
print_string:
    mov ah, 0x0e    ; BIOS teletype function
.next_char:
    lodsb           ; Load byte at SI into AL, increment SI
    cmp al, 0       ; Check for null terminator
    je .done        ; If null, we're done
    int 0x10        ; Print character in AL
    jmp .next_char  ; Continue with next character
.done:
    ret

; Data section
welcome_msg    db 'Welcome to Quasar-OS!', 0x0d, 0x0a, 0x0a, 0
version_msg    db 'Bootloader v1.0 - Built from scratch', 0x0d, 0x0a, 0x0a, 0
instruction_msg db 'System halted. This is just the beginning...', 0x0d, 0x0a, 0
loading_msg    db 'Loading kernel...', 0x0d, 0x0a, 0

; Fill the rest of the 512-byte sector with zeros
times 510-($-$$) db 0

; Boot signature (required for BIOS to recognize this as bootable)
dw 0xAA55