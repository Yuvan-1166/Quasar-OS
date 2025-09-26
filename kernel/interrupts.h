// Quasar-OS Interrupt System Header
// Defines structures and functions for interrupt handling

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

// Forward declarations for terminal functions
void terminal_writestring(const char* data);
void terminal_write_hex(unsigned int value);

// IDT entry structure
struct idt_entry {
    unsigned short base_lo;  // Lower 16 bits of handler address
    unsigned short sel;      // Kernel segment selector
    unsigned char always0;   // Always zero
    unsigned char flags;     // Type and attributes
    unsigned short base_hi;  // Upper 16 bits of handler address
} __attribute__((packed));

// IDT pointer structure
struct idt_ptr {
    unsigned short limit;    // Size of IDT
    unsigned int base;       // Address of IDT
} __attribute__((packed));

// Register state structure (pushed by ISR)
struct registers {
    unsigned int ds;         // Data segment selector
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    unsigned int int_no, err_code;    // Interrupt number and error code
    unsigned int eip, cs, eflags, useresp, ss; // Pushed by processor automatically
};

// Interrupt handler function type
typedef void (*interrupt_handler_t)(struct registers*);

// Function prototypes
void init_interrupts(void);
void set_idt_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags);
void init_pic(void);
void register_interrupt_handler(unsigned char n, interrupt_handler_t handler);

// Port I/O functions
void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);

// Interrupt handlers - defined in interrupt.asm
extern void isr0(void);   extern void isr1(void);   extern void isr2(void);   extern void isr3(void);
extern void isr4(void);   extern void isr5(void);   extern void isr6(void);   extern void isr7(void);
extern void isr8(void);   extern void isr10(void);  extern void isr11(void);  extern void isr12(void);
extern void isr13(void);  extern void isr14(void);  extern void isr16(void);  extern void isr17(void);
extern void isr18(void);  extern void isr19(void);

// IRQ handlers
extern void irq0(void);   extern void irq1(void);   extern void irq2(void);   extern void irq3(void);
extern void irq4(void);   extern void irq5(void);   extern void irq6(void);   extern void irq7(void);
extern void irq8(void);   extern void irq9(void);   extern void irq10(void);  extern void irq11(void);
extern void irq12(void);  extern void irq13(void);  extern void irq14(void);  extern void irq15(void);

// Assembly functions
extern void idt_flush(unsigned int);

// Handler functions called from assembly
void interrupt_handler(struct registers regs);
void irq_handler(struct registers regs);

#endif