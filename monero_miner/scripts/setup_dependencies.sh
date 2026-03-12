#!/bin/bash
set -e

# Directory for external dependencies
EXTERNAL_DIR="$(pwd)/external"
mkdir -p "$EXTERNAL_DIR"

echo "Setting up dependencies in $EXTERNAL_DIR..."

# 1. RandomX
if [ ! -d "$EXTERNAL_DIR/RandomX" ]; then
    echo "Cloning RandomX..."
    git clone https://github.com/tevador/RandomX.git "$EXTERNAL_DIR/RandomX"
    cd "$EXTERNAL_DIR/RandomX"
    mkdir -p build && cd build
    cmake ..
    make -j$(nproc)
    echo "RandomX built successfully."
    cd ../../../
else
    echo "RandomX already exists, skipping clone."
fi

# 2. nlohmann/json (Single Header)
if [ ! -f "$EXTERNAL_DIR/json.hpp" ]; then
    echo "Downloading nlohmann/json..."
    curl -L https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp -o "$EXTERNAL_DIR/json.hpp"
    echo "nlohmann/json downloaded."
else
    echo "json.hpp already exists."
fi

# 3. libcurl (Check if installed, else guide)
if command -v curl-config >/dev/null 2>&1; then
    echo "libcurl is already installed on the system."
else
    echo "WARNING: libcurl not found. Please install it (e.g., 'sudo apt install libcurl4-openssl-dev' on Ubuntu/Debian)."
fi

echo "Dependency setup complete."
