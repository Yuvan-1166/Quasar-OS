# Quasar-OS Makefile
# Build system for our operating system

# Directories
BOOT_DIR = boot
KERNEL_DIR = kernel
BUILD_DIR = build

# Tools
ASM = nasm
CC = gcc
LD = ld
QEMU = qemu-system-i386

# Flags
ASM_FLAGS = -f bin
ASM_ELF_FLAGS = -f elf32
CC_FLAGS = -m32 -ffreestanding -fno-stack-protector -fno-builtin -nostdlib -c
LD_FLAGS = -m elf_i386 -T $(KERNEL_DIR)/linker.ld

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build enhanced bootloader
$(BUILD_DIR)/bootloader.bin: $(BOOT_DIR)/bootloader.asm | $(BUILD_DIR)
	@echo "Building enhanced bootloader..."
	$(ASM) $(ASM_FLAGS) $< -o $@
	@echo "Enhanced bootloader built successfully!"

# Build kernel entry point
$(BUILD_DIR)/entry.o: $(KERNEL_DIR)/entry.asm | $(BUILD_DIR)
	@echo "Building kernel entry point..."
	$(ASM) $(ASM_ELF_FLAGS) $< -o $@

# Build kernel main
$(BUILD_DIR)/kernel.o: $(KERNEL_DIR)/kernel.c | $(BUILD_DIR)
	@echo "Building kernel..."
	$(CC) $(CC_FLAGS) $< -o $@

# Link kernel
$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/entry.o $(BUILD_DIR)/kernel.o
	@echo "Linking kernel..."
	$(LD) $(LD_FLAGS) $^ -o $(BUILD_DIR)/kernel.elf
	objcopy -O binary $(BUILD_DIR)/kernel.elf $@
	@echo "Kernel linked and converted to binary format"

# Create bootable disk image
$(BUILD_DIR)/quasar.img: $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin
	@echo "Creating disk image..."
	# Create 1.44MB floppy image
	dd if=/dev/zero of=$@ bs=512 count=2880
	# Write bootloader to first sector
	dd if=$(BUILD_DIR)/bootloader.bin of=$@ bs=512 count=1 conv=notrunc
	# Write kernel starting from sector 2
	dd if=$(BUILD_DIR)/kernel.bin of=$@ bs=512 seek=1 conv=notrunc
	@echo "Disk image created successfully!"

# Build everything
all: $(BUILD_DIR)/quasar.img
	@echo "Quasar-OS build complete!"

# Run in QEMU
run: $(BUILD_DIR)/quasar.img
	@echo "Starting Quasar-OS in QEMU..."
	@echo "QEMU window should open. If not visible, check your display settings."
	$(QEMU) -drive file=$<,format=raw,media=disk -monitor stdio

# Run with no graphics (text only)
run-nographic: $(BUILD_DIR)/quasar.img
	@echo "Starting Quasar-OS in QEMU (text mode)..."
	$(QEMU) -drive file=$<,format=raw,media=disk -nographic

# Run with GUI (if display available)
run-gui: $(BUILD_DIR)/quasar.img
	@echo "Starting Quasar-OS in QEMU with GUI..."
	$(QEMU) -drive file=$<,format=raw,media=disk

# Debug with QEMU (with GDB support)
debug: $(BUILD_DIR)/quasar.img
	@echo "Starting Quasar-OS in debug mode..."
	$(QEMU) -drive file=$<,format=raw,media=disk -s -S

# Clean build files
clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_DIR)
	@echo "Clean complete!"

# Show help
help:
	@echo "Quasar-OS Build System v2.0"
	@echo "============================"
	@echo "Available targets:"
	@echo "  all          - Build the complete OS (bootloader + kernel)"
	@echo "  run          - Build and run in QEMU with monitor"
	@echo "  run-gui      - Build and run in QEMU with GUI"
	@echo "  run-nographic- Build and run in QEMU (text mode only)"
	@echo "  debug        - Build and run with GDB debugging"
	@echo "  clean        - Remove all build files"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Build components:"
	@echo "  bootloader - Enhanced bootloader with kernel loading"
	@echo "  kernel     - Basic C kernel with VGA text output"

# Declare phony targets
.PHONY: all run run-gui run-nographic debug clean help