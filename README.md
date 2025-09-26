# Quasar-OS

Quasar-OS is a lightweight, modular operating system designed for flexibility and performance. Built from scratch, it aims to provide a simple yet powerful environment for learning, experimentation, and development.

## 🚀 Development Roadmap

Building an operating system from scratch is a complex but rewarding journey. Here's our comprehensive step-by-step approach:

### Phase 1: Foundation & Bootloader (Weeks 1-2)
1. **Development Environment Setup**
   - Set up cross-compilation toolchain (GCC, Binutils)
   - Configure build system (Make/CMake)
   - Set up debugging tools (GDB, QEMU)

2. **Bootloader Development**
   - Create a simple bootloader (16-bit real mode)
   - Implement basic boot sector code
   - Add support for loading kernel from disk
   - Transition from 16-bit to 32-bit protected mode

### Phase 2: Basic Kernel (Weeks 3-6)
3. **Kernel Entry Point**
   - Set up initial kernel structure
   - Implement basic kernel entry point
   - Set up stack and basic memory layout

4. **Display & Output**
   - Implement VGA text mode driver
   - Create basic printf/printk functionality
   - Add color support and screen management

5. **Memory Management Foundation**
   - Implement basic physical memory manager
   - Set up page tables and virtual memory
   - Create kernel heap allocator (kmalloc/kfree)

### Phase 3: Core System Services (Weeks 7-12)
6. **Interrupt Handling**
   - Set up Interrupt Descriptor Table (IDT)
   - Implement interrupt service routines
   - Add keyboard interrupt handler

7. **Process Management**
   - Design process control block (PCB) structure
   - Implement basic scheduler (round-robin)
   - Add process creation and termination
   - Implement context switching

8. **System Calls Interface**
   - Design system call interface
   - Implement basic system calls (exit, fork, exec)
   - Add user/kernel mode separation

### Phase 4: File System & Storage (Weeks 13-16)
9. **Storage Abstraction**
   - Implement basic ATA/IDE driver
   - Create block device interface
   - Add disk I/O operations

10. **File System Implementation**
    - Design simple file system (similar to FAT or ext2)
    - Implement directory structure
    - Add file operations (open, read, write, close)

### Phase 5: Device Management (Weeks 17-20)
11. **Device Drivers Framework**
    - Create device driver architecture
    - Implement timer driver
    - Add basic network interface (if ambitious)

12. **Advanced I/O**
    - Implement buffered I/O
    - Add DMA support
    - Create device file abstraction (/dev)

### Phase 6: User Space & Shell (Weeks 21-24)
13. **User Space Programs**
    - Create basic C library subset
    - Implement program loader (ELF support)
    - Add dynamic linking support

14. **Shell Implementation**
    - Create basic command-line interface
    - Implement built-in commands
    - Add program execution capabilities

### Phase 7: Advanced Features (Weeks 25+)
15. **Networking Stack** (Optional)
    - Implement basic TCP/IP stack
    - Add socket interface
    - Create network utilities

16. **Graphics Subsystem** (Optional)
    - Implement framebuffer support
    - Add basic graphics primitives
    - Create simple GUI framework

17. **Security & Permissions**
    - Implement user/group system
    - Add file permissions
    - Create access control mechanisms

## 📁 Project Structure
```
Quasar-OS/
├── boot/                 # Bootloader code
├── kernel/               # Kernel source code
│   ├── arch/            # Architecture-specific code
│   ├── drivers/         # Device drivers
│   ├── fs/              # File system
│   ├── mm/              # Memory management
│   └── proc/            # Process management
├── lib/                 # Kernel libraries
├── userspace/           # User space programs
├── tools/               # Build tools and utilities
├── docs/                # Documentation
└── tests/               # Test suites
```

## 🛠️ Prerequisites & Tools

### Required Tools
- **GCC Cross-Compiler**: For compiling kernel code
- **NASM**: Assembly language compiler
- **QEMU**: For testing and emulation
- **GDB**: For debugging
- **Make**: Build automation
- **Git**: Version control

### Development Environment
```bash
# Install required packages (Ubuntu/Debian)
sudo apt update
sudo apt install build-essential nasm qemu-system-x86 gdb git

# Set up cross-compiler (we'll create a script for this)
./tools/setup-toolchain.sh
```

## 🚀 Quick Start

```bash
# Clone the repository
git clone https://github.com/Yuvan-1166/Quasar-OS.git
cd Quasar-OS

# Build the OS
make all

# Run in QEMU
make run

# Debug with GDB
make debug
```

## 📚 Learning Resources
- **Books**: 
  - "Operating System Concepts" by Silberschatz
  - "Modern Operating Systems" by Tanenbaum
  - "Operating Systems: Design and Implementation" by Tanenbaum
- **Online**: OSDev Wiki, Bran's Kernel Development Tutorial
- **References**: Intel Software Developer Manuals, AMD64 Architecture

## 🎯 Current Status
- [ ] Phase 1: Foundation & Bootloader
- [ ] Phase 2: Basic Kernel
- [ ] Phase 3: Core System Services
- [ ] Phase 4: File System & Storage
- [ ] Phase 5: Device Management
- [ ] Phase 6: User Space & Shell
- [ ] Phase 7: Advanced Features

## 🤝 Contributing
This is a learning project, but contributions and suggestions are welcome! Please read our contributing guidelines before submitting pull requests.

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments
- OSDev Community
- Various open-source operating systems for inspiration
- Educational resources and tutorials that guide this journey