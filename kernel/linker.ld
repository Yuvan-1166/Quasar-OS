/* Quasar-OS Kernel Linker Script */
/* Defines memory layout for the kernel */

ENTRY(_start)           /* Entry point is _start symbol */

/* Define where the kernel should be loaded */
SECTIONS
{
    /* Kernel starts at 8000h (32KB) - same as KERNEL_LOAD_ADDR in bootloader */
    . = 0x8000;
    
    /* Code section */
    .text : {
        *(.text)        /* All text sections from all object files */
    }
    
    /* Read-only data section */
    .rodata : {
        *(.rodata)      /* All read-only data sections */
    }
    
    /* Initialized data section */
    .data : {
        *(.data)        /* All data sections */
    }
    
    /* Uninitialized data section */
    .bss : {
        *(.bss)         /* All BSS sections */
    }
}