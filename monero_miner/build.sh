#!/bin/bash
set -e

echo "========================================"
echo "   AI Monero Miner - Auto Build Script  "
echo "========================================"

mkdir -p build
cd build

echo "[INFO] Configuring project..."
cmake ..

echo "[INFO] Building project..."
make -j$(nproc)

echo "[SUCCESS] Build complete!"
echo "Binary is located in build/bin/ai-monero-miner-pro"
