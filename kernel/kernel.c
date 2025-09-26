// Quasar-OS Kernel Entry Point
// Basic kernel that runs in 32-bit protected mode with interrupt support

#include "interrupts.h"
#include "timer.h"
#include "keyboard.h"

// VGA text mode buffer
#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Colors for VGA text mode
#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHT_GREY 7
#define COLOR_DARK_GREY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

// Make color byte from foreground and background
#define VGA_COLOR(fg, bg) ((bg << 4) | (fg & 0x0F))

// Make VGA entry from character and color
#define VGA_ENTRY(ch, color) ((unsigned short)ch | ((unsigned short)color << 8))

// Global variables
static unsigned int cursor_row = 0;
static unsigned int cursor_col = 0;
static unsigned char current_color = VGA_COLOR(COLOR_LIGHT_GREEN, COLOR_BLACK);

// Function prototypes
void kernel_main(void);
void terminal_initialize(void);
void terminal_clear(void);
void terminal_putchar(char c);
void terminal_write(const char* data, unsigned int size);
void terminal_writestring(const char* data);
void terminal_setcolor(unsigned char color);
void terminal_write_hex(unsigned int value);
void terminal_write_dec(unsigned int value);
unsigned int strlen(const char* str);

// Kernel entry point (called by bootloader)
void kernel_main(void) {
    // Initialize terminal
    terminal_initialize();
    
    // Display welcome message
    terminal_setcolor(VGA_COLOR(COLOR_LIGHT_GREEN, COLOR_BLACK));
    terminal_writestring("Welcome to Quasar-OS Kernel v2.0!\n");
    terminal_writestring("===================================\n\n");
    
    terminal_setcolor(VGA_COLOR(COLOR_WHITE, COLOR_BLACK));
    terminal_writestring("Kernel loaded successfully in 32-bit protected mode\n");
    
    // Initialize interrupt system
    terminal_setcolor(VGA_COLOR(COLOR_YELLOW, COLOR_BLACK));
    terminal_writestring("\nInitializing interrupt system...\n");
    init_interrupts();
    terminal_writestring("Interrupts initialized successfully!\n");
    
    // Initialize timer (100 Hz)
    terminal_writestring("\nInitializing system timer...\n");
    init_timer(100);
    
    // Initialize keyboard
    terminal_writestring("\nInitializing keyboard driver...\n");
    init_keyboard();
    
    terminal_setcolor(VGA_COLOR(COLOR_LIGHT_CYAN, COLOR_BLACK));
    terminal_writestring("\nSystem Status: Running with interrupts enabled\n");
    terminal_writestring("Architecture: x86 32-bit\n");
    terminal_writestring("Features: Timer, Keyboard, VGA Text Mode\n\n");
    
    terminal_setcolor(VGA_COLOR(COLOR_LIGHT_GREEN, COLOR_BLACK));
    terminal_writestring("Quasar-OS is now interactive! Try typing something:\n");
    terminal_writestring("> ");
    
    // Enable interrupts
    __asm__ volatile ("sti");
    
    // Infinite loop - kernel is running and responsive to interrupts
    while(1) {
        // In a real OS, this would be the idle loop
        // We could implement power management here
        __asm__ volatile ("hlt"); // Halt until next interrupt
    }
}

// Initialize the terminal
void terminal_initialize(void) {
    terminal_clear();
    cursor_row = 0;
    cursor_col = 0;
    current_color = VGA_COLOR(COLOR_LIGHT_GREY, COLOR_BLACK);
}

// Clear the terminal screen
void terminal_clear(void) {
    unsigned short* vga_buffer = (unsigned short*) VGA_MEMORY;
    unsigned short blank = VGA_ENTRY(' ', current_color);
    
    for (unsigned int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }
}

// Set terminal color
void terminal_setcolor(unsigned char color) {
    current_color = color;
}

// Put a character on screen
void terminal_putchar(char c) {
    unsigned short* vga_buffer = (unsigned short*) VGA_MEMORY;
    
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
    } else {
        unsigned int index = cursor_row * VGA_WIDTH + cursor_col;
        vga_buffer[index] = VGA_ENTRY(c, current_color);
        cursor_col++;
    }
    
    // Handle line wrap
    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
    }
    
    // Handle screen scroll (simple version - just wrap to top)
    if (cursor_row >= VGA_HEIGHT) {
        cursor_row = 0;
        cursor_col = 0;
    }
}

// Write data to terminal
void terminal_write(const char* data, unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

// Write string to terminal
void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

// Get string length
unsigned int strlen(const char* str) {
    unsigned int len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

// Write hexadecimal number
void terminal_write_hex(unsigned int value) {
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[9];  // 8 hex digits + null terminator
    buffer[8] = '\0';
    
    for (int i = 7; i >= 0; i--) {
        buffer[i] = hex_chars[value & 0xF];
        value >>= 4;
    }
    
    terminal_writestring("0x");
    terminal_writestring(buffer);
}

// Write decimal number
void terminal_write_dec(unsigned int value) {
    if (value == 0) {
        terminal_writestring("0");
        return;
    }
    
    char buffer[11];  // Max 10 digits for 32-bit int + null terminator
    int pos = 10;
    buffer[pos] = '\0';
    
    while (value > 0 && pos > 0) {
        pos--;
        buffer[pos] = '0' + (value % 10);
        value /= 10;
    }
    
    terminal_writestring(&buffer[pos]);
}