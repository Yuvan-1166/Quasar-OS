#!/bin/bash
# Quasar-OS Development Environment Setup Script

echo "Setting up Quasar-OS development environment..."

# Check if running on supported system
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "✓ Linux detected"
else
    echo "❌ This script is designed for Linux systems"
    exit 1
fi

# Detect package manager
if command -v apt &> /dev/null; then
    PKG_MANAGER="apt"
    INSTALL_CMD="sudo apt install -y"
    UPDATE_CMD="sudo apt update"
elif command -v dnf &> /dev/null; then
    PKG_MANAGER="dnf"
    INSTALL_CMD="sudo dnf install -y"
    UPDATE_CMD="sudo dnf update"
elif command -v pacman &> /dev/null; then
    PKG_MANAGER="pacman"
    INSTALL_CMD="sudo pacman -S --noconfirm"
    UPDATE_CMD="sudo pacman -Sy"
else
    echo "❌ Unsupported package manager"
    exit 1
fi

echo "✓ Package manager: $PKG_MANAGER"

# Update package database
echo "Updating package database..."
$UPDATE_CMD

# Install required packages
echo "Installing required packages..."
case $PKG_MANAGER in
    "apt")
        $INSTALL_CMD build-essential nasm qemu-system-x86 gdb git
        ;;
    "dnf")
        $INSTALL_CMD gcc gcc-c++ make nasm qemu-system-x86 gdb git
        ;;
    "pacman")
        $INSTALL_CMD base-devel nasm qemu gdb git
        ;;
esac

# Verify installations
echo "Verifying installations..."
TOOLS=("gcc" "nasm" "qemu-system-i386" "make" "gdb" "git")
for tool in "${TOOLS[@]}"; do
    if command -v "$tool" &> /dev/null; then
        echo "✓ $tool installed"
    else
        echo "❌ $tool not found"
    fi
done

echo ""
echo "🎉 Development environment setup complete!"
echo ""
echo "Next steps:"
echo "1. Run 'make all' to build Quasar-OS"
echo "2. Run 'make run' to test in QEMU"
echo "3. Run 'make help' to see all available commands"