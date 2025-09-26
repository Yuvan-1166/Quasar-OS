// Quasar-OS Kernel Entry Point
// Basic kernel that runs in 32-bit protected mode

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
unsigned int strlen(const char* str);

// Kernel entry point (called by bootloader)
void kernel_main(void) {
    // Initialize terminal
    terminal_initialize();
    
    // Display welcome message
    terminal_setcolor(VGA_COLOR(COLOR_LIGHT_GREEN, COLOR_BLACK));
    terminal_writestring("Welcome to Quasar-OS Kernel!\n");
    terminal_writestring("=============================\n\n");
    
    terminal_setcolor(VGA_COLOR(COLOR_WHITE, COLOR_BLACK));
    terminal_writestring("Kernel loaded successfully in 32-bit protected mode\n");
    terminal_writestring("Memory management: Not implemented yet\n");
    terminal_writestring("Process management: Not implemented yet\n");
    terminal_writestring("File system: Not implemented yet\n\n");
    
    terminal_setcolor(VGA_COLOR(COLOR_YELLOW, COLOR_BLACK));
    terminal_writestring("System Status: Running\n");
    terminal_writestring("Architecture: x86 32-bit\n");
    terminal_writestring("Boot Stage: Kernel initialized\n\n");
    
    terminal_setcolor(VGA_COLOR(COLOR_LIGHT_CYAN, COLOR_BLACK));
    terminal_writestring("Next steps:\n");
    terminal_writestring("1. Implement interrupt handling\n");
    terminal_writestring("2. Set up memory management\n");
    terminal_writestring("3. Create process scheduler\n\n");
    
    terminal_setcolor(VGA_COLOR(COLOR_LIGHT_RED, COLOR_BLACK));
    terminal_writestring("Kernel halted. System is running but idle.\n");
    
    // Infinite loop - kernel is running but doing nothing
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