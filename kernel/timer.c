// Quasar-OS Timer Driver
// Handles system timer (PIT - Programmable Interval Timer)

#include "timer.h"
#include "interrupts.h"

// Timer frequency and tick counter
static unsigned int timer_frequency = 0;
static unsigned int tick_count = 0;

// Initialize the timer
void init_timer(unsigned int frequency) {
    // Register the timer callback
    register_interrupt_handler(IRQ0, &timer_callback);
    
    // Set frequency
    timer_frequency = frequency;
    
    // Calculate the divisor for PIT
    unsigned int divisor = 1193180 / frequency;   // 1193180 Hz is PIT base frequency
    
    // Send the command byte
    outb(0x43, 0x36);  // Command: channel 0, lobyte/hibyte, rate generator
    
    // Send the divisor
    unsigned char l = (unsigned char)(divisor & 0xFF);
    unsigned char h = (unsigned char)((divisor >> 8) & 0xFF);
    
    outb(0x40, l);     // Send low byte
    outb(0x40, h);     // Send high byte
    
    terminal_writestring("Timer initialized at ");
    terminal_write_dec(frequency);
    terminal_writestring(" Hz\n");
}

// Timer interrupt callback
void timer_callback(struct registers* regs) {
    tick_count++;
    
    // Display a message every second (assuming 100 Hz timer)
    if (tick_count % timer_frequency == 0) {
        terminal_writestring("Timer tick: ");
        terminal_write_dec(tick_count / timer_frequency);
        terminal_writestring(" seconds\n");
    }
}

// Get current tick count
unsigned int get_tick_count(void) {
    return tick_count;
}

// Get uptime in seconds
unsigned int get_uptime(void) {
    return tick_count / timer_frequency;
}