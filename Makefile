# Quasar-OS Makefile
# Build system for our operating system

# Directories
BOOT_DIR = boot
KERNEL_DIR = kernel
BUILD_DIR = build

# Tools
ASM = nasm
QEMU = qemu-system-i386

# Flags
ASM_FLAGS = -f bin

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build bootloader
$(BUILD_DIR)/boot.bin: $(BOOT_DIR)/boot.asm | $(BUILD_DIR)
	@echo "Building bootloader..."
	$(ASM) $(ASM_FLAGS) $< -o $@
	@echo "Bootloader built successfully!"

# Create bootable disk image
$(BUILD_DIR)/quasar.img: $(BUILD_DIR)/boot.bin
	@echo "Creating disk image..."
	dd if=$< of=$@ bs=512 count=1
	dd if=/dev/zero of=$@ bs=512 count=2879 seek=1
	@echo "Disk image created successfully!"

# Build everything
all: $(BUILD_DIR)/quasar.img
	@echo "Quasar-OS build complete!"

# Run in QEMU
run: $(BUILD_DIR)/quasar.img
	@echo "Starting Quasar-OS in QEMU..."
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
	@echo "Quasar-OS Build System"
	@echo "====================="
	@echo "Available targets:"
	@echo "  all     - Build the complete OS"
	@echo "  run     - Build and run in QEMU"
	@echo "  debug   - Build and run with GDB debugging"
	@echo "  clean   - Remove all build files"
	@echo "  help    - Show this help message"

# Declare phony targets
.PHONY: all run debug clean help