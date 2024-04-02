#!/bin/bash

# Update package lists
sudo apt update

# Upgrade system
sudo apt upgrade

# Install MinGW-w64
sudo apt install -y mingw-w64

# Verify installation
if ! command -v i686-w64-mingw32-gcc &>/dev/null; then
    echo "MinGW-w64 installation failed."
    exit 1
fi

# Optionally install any additional dependencies here

echo "MinGW-w64 and dependencies installed successfully."