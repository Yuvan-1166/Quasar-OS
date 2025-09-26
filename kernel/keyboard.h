// Quasar-OS Keyboard Driver Header
// Keyboard input handling and key mapping

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "interrupts.h"

// IRQ numbers
#define IRQ1 33  // Keyboard interrupt

// Function prototypes
void init_keyboard(void);
void keyboard_callback(struct registers* regs);

#endif