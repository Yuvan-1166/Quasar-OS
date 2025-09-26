// Quasar-OS Timer Driver Header
// Timer and system clock functionality

#ifndef TIMER_H
#define TIMER_H

#include "interrupts.h"

// IRQ numbers
#define IRQ0 32  // Timer interrupt

// Function prototypes
void init_timer(unsigned int frequency);
void timer_callback(struct registers* regs);
unsigned int get_tick_count(void);
unsigned int get_uptime(void);

// Forward declarations for terminal functions
void terminal_write_dec(unsigned int value);

#endif