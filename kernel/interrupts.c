// Quasar-OS Interrupt Management
// Handles interrupt descriptor table and interrupt service routines

#include "interrupts.h"

// IDT structure - 256 entries for all possible interrupts
static struct idt_entry idt_entries[256];
static struct idt_ptr idt_ptr;

// Interrupt handlers array
static interrupt_handler_t interrupt_handlers[256];

// Initialize the interrupt system
void init_interrupts(void) {
    // Clear all interrupt handlers
    for (int i = 0; i < 256; i++) {
        interrupt_handlers[i] = 0;
    }
    
    // Set up IDT pointer
    idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
    idt_ptr.base = (unsigned int)&idt_entries;
    
    // Clear IDT entries
    for (int i = 0; i < 256; i++) {
        set_idt_gate(i, 0, 0x08, 0);
    }
    
    // Set up exception handlers (interrupts 0-31)
    set_idt_gate(0, (unsigned int)isr0, 0x08, 0x8E);   // Division by zero
    set_idt_gate(1, (unsigned int)isr1, 0x08, 0x8E);   // Debug
    set_idt_gate(2, (unsigned int)isr2, 0x08, 0x8E);   // Non-maskable interrupt
    set_idt_gate(3, (unsigned int)isr3, 0x08, 0x8E);   // Breakpoint
    set_idt_gate(4, (unsigned int)isr4, 0x08, 0x8E);   // Overflow
    set_idt_gate(5, (unsigned int)isr5, 0x08, 0x8E);   // Bound range exceeded
    set_idt_gate(6, (unsigned int)isr6, 0x08, 0x8E);   // Invalid opcode
    set_idt_gate(7, (unsigned int)isr7, 0x08, 0x8E);   // Device not available
    set_idt_gate(8, (unsigned int)isr8, 0x08, 0x8E);   // Double fault
    set_idt_gate(10, (unsigned int)isr10, 0x08, 0x8E); // Invalid TSS
    set_idt_gate(11, (unsigned int)isr11, 0x08, 0x8E); // Segment not present
    set_idt_gate(12, (unsigned int)isr12, 0x08, 0x8E); // Stack fault
    set_idt_gate(13, (unsigned int)isr13, 0x08, 0x8E); // General protection fault
    set_idt_gate(14, (unsigned int)isr14, 0x08, 0x8E); // Page fault
    set_idt_gate(16, (unsigned int)isr16, 0x08, 0x8E); // FPU error
    set_idt_gate(17, (unsigned int)isr17, 0x08, 0x8E); // Alignment check
    set_idt_gate(18, (unsigned int)isr18, 0x08, 0x8E); // Machine check
    set_idt_gate(19, (unsigned int)isr19, 0x08, 0x8E); // SIMD floating point
    
    // Set up hardware interrupt handlers (IRQs 32-47)
    set_idt_gate(32, (unsigned int)irq0, 0x08, 0x8E);  // Timer
    set_idt_gate(33, (unsigned int)irq1, 0x08, 0x8E);  // Keyboard
    set_idt_gate(34, (unsigned int)irq2, 0x08, 0x8E);  // Cascade
    set_idt_gate(35, (unsigned int)irq3, 0x08, 0x8E);  // COM2
    set_idt_gate(36, (unsigned int)irq4, 0x08, 0x8E);  // COM1
    set_idt_gate(37, (unsigned int)irq5, 0x08, 0x8E);  // LPT2
    set_idt_gate(38, (unsigned int)irq6, 0x08, 0x8E);  // Floppy
    set_idt_gate(39, (unsigned int)irq7, 0x08, 0x8E);  // LPT1
    set_idt_gate(40, (unsigned int)irq8, 0x08, 0x8E);  // RTC
    set_idt_gate(41, (unsigned int)irq9, 0x08, 0x8E);  // Free
    set_idt_gate(42, (unsigned int)irq10, 0x08, 0x8E); // Free
    set_idt_gate(43, (unsigned int)irq11, 0x08, 0x8E); // Free
    set_idt_gate(44, (unsigned int)irq12, 0x08, 0x8E); // PS2 Mouse
    set_idt_gate(45, (unsigned int)irq13, 0x08, 0x8E); // FPU
    set_idt_gate(46, (unsigned int)irq14, 0x08, 0x8E); // Primary ATA
    set_idt_gate(47, (unsigned int)irq15, 0x08, 0x8E); // Secondary ATA
    
    // Load the IDT
    idt_flush((unsigned int)&idt_ptr);
    
    // Initialize PIC (Programmable Interrupt Controller)
    init_pic();
}

// Set up an IDT gate
void set_idt_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags) {
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}

// Initialize the PIC
void init_pic(void) {
    // Send ICW1: Initialize command
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    
    // Send ICW2: Remap IRQ0-7 to interrupts 32-39, IRQ8-15 to interrupts 40-47
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    
    // Send ICW3: Tell master PIC there's a slave at IRQ2, tell slave its cascade identity
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    
    // Send ICW4: 8086 mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    
    // Enable all IRQs
    outb(0x21, 0x00);
    outb(0xA1, 0x00);
}

// Register an interrupt handler
void register_interrupt_handler(unsigned char n, interrupt_handler_t handler) {
    interrupt_handlers[n] = handler;
}

// Common interrupt handler - called from assembly stubs
void interrupt_handler(struct registers regs) {
    // Check if we have a registered handler
    if (interrupt_handlers[regs.int_no] != 0) {
        interrupt_handler_t handler = interrupt_handlers[regs.int_no];
        handler(&regs);
    } else {
        // Unhandled interrupt
        terminal_writestring("Unhandled interrupt: ");
        terminal_write_hex(regs.int_no);
        terminal_writestring("\n");
    }
}

// IRQ handler - called for hardware interrupts
void irq_handler(struct registers regs) {
    // Send EOI (End of Interrupt) to PICs
    if (regs.int_no >= 40) {
        outb(0xA0, 0x20); // Send EOI to slave
    }
    outb(0x20, 0x20); // Send EOI to master
    
    // Call the registered handler
    if (interrupt_handlers[regs.int_no] != 0) {
        interrupt_handler_t handler = interrupt_handlers[regs.int_no];
        handler(&regs);
    }
}

// Port I/O functions
void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}