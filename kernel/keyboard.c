// Quasar-OS Keyboard Driver
// Handles keyboard input and key mapping

#include "keyboard.h"
#include "interrupts.h"

// US QWERTY keyboard layout
static unsigned char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',     // 0-9
    '9', '0', '-', '=', '\b',    // Backspace
    '\t',            // Tab
    'q', 'w', 'e', 'r',    // 16-19
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',    // Enter key
    0,            // 29   - Control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',    // 30-39
    '\'', '`',   0,        // Left shift
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',            // 49
    'm', ',', '.', '/',   0,                // Right shift
    '*',
    0,    // Alt
    ' ',    // Space bar
    0,    // Caps lock
    0,    // 59 - F1 key ... >
    0,   0,   0,   0,   0,   0,   0,   0,
    0,    // < ... F10
    0,    // 69 - Num lock
    0,    // Scroll Lock
    0,    // Home key
    0,    // Up Arrow
    0,    // Page Up
    '-',
    0,    // Left Arrow
    0,
    0,    // Right Arrow
    '+',
    0,    // 79 - End key
    0,    // Down Arrow
    0,    // Page Down
    0,    // Insert Key
    0,    // Delete Key
    0,   0,   0,
    0,    // F11 Key
    0,    // F12 Key
    0,    // All other keys are undefined
};

// Shift key states
static int shift_pressed = 0;
static int ctrl_pressed = 0;
static int alt_pressed = 0;

// Initialize keyboard driver
void init_keyboard(void) {
    // Register keyboard interrupt handler
    register_interrupt_handler(IRQ1, &keyboard_callback);
    
    terminal_writestring("Keyboard driver initialized\n");
    terminal_writestring("You can now type! Try pressing some keys...\n");
}

// Keyboard interrupt callback
void keyboard_callback(struct registers* regs) {
    // Read scancode from keyboard
    unsigned char scancode = inb(0x60);
    
    // Check if key was released (bit 7 set)
    if (scancode & 0x80) {
        // Key released
        scancode &= 0x7F;  // Remove released bit
        
        // Handle modifier keys
        if (scancode == 42 || scancode == 54) {  // Shift keys
            shift_pressed = 0;
        }
        else if (scancode == 29) {  // Ctrl key
            ctrl_pressed = 0;
        }
        else if (scancode == 56) {  // Alt key
            alt_pressed = 0;
        }
    }
    else {
        // Key pressed
        
        // Handle modifier keys
        if (scancode == 42 || scancode == 54) {  // Shift keys
            shift_pressed = 1;
            return;
        }
        else if (scancode == 29) {  // Ctrl key
            ctrl_pressed = 1;
            return;
        }
        else if (scancode == 56) {  // Alt key
            alt_pressed = 1;
            return;
        }
        
        // Handle special key combinations
        if (ctrl_pressed && scancode == 46) {  // Ctrl+C
            terminal_writestring("\n^C (Ctrl+C detected)\n");
            return;
        }
        
        // Convert scancode to ASCII
        if (scancode < 128) {
            char key = keyboard_map[scancode];
            
            if (key != 0) {
                // Handle shift modifier for letters
                if (shift_pressed) {
                    if (key >= 'a' && key <= 'z') {
                        key = key - 'a' + 'A';  // Convert to uppercase
                    }
                    else {
                        // Handle shifted symbols
                        switch(key) {
                            case '1': key = '!'; break;
                            case '2': key = '@'; break;
                            case '3': key = '#'; break;
                            case '4': key = '$'; break;
                            case '5': key = '%'; break;
                            case '6': key = '^'; break;
                            case '7': key = '&'; break;
                            case '8': key = '*'; break;
                            case '9': key = '('; break;
                            case '0': key = ')'; break;
                            case '-': key = '_'; break;
                            case '=': key = '+'; break;
                            case '[': key = '{'; break;
                            case ']': key = '}'; break;
                            case '\\': key = '|'; break;
                            case ';': key = ':'; break;
                            case '\'': key = '"'; break;
                            case '`': key = '~'; break;
                            case ',': key = '<'; break;
                            case '.': key = '>'; break;
                            case '/': key = '?'; break;
                        }
                    }
                }
                
                // Display the character
                if (key == '\n') {
                    terminal_writestring("\n");
                }
                else if (key == '\b') {
                    // Simple backspace handling
                    terminal_writestring("\b \b");
                }
                else if (key == '\t') {
                    terminal_writestring("    ");  // 4 spaces for tab
                }
                else {
                    char str[2] = {key, '\0'};
                    terminal_writestring(str);
                }
            }
        }
    }
}