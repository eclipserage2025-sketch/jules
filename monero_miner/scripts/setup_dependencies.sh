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

# 3. Dear ImGui
if [ ! -d "$EXTERNAL_DIR/imgui" ]; then
    echo "Cloning Dear ImGui..."
    git clone https://github.com/ocornut/imgui.git "$EXTERNAL_DIR/imgui"
    cd "$EXTERNAL_DIR/imgui"
    git checkout v1.90.1
    cd ../../
else
    echo "Dear ImGui already exists."
fi

# 4. GLFW (for ImGui)
if [ ! -d "$EXTERNAL_DIR/glfw" ]; then
    echo "Cloning GLFW..."
    git clone https://github.com/glfw/glfw.git "$EXTERNAL_DIR/glfw"
    cd "$EXTERNAL_DIR/glfw"
    mkdir -p build && cd build
    # Disable Wayland to avoid dependency issues in this environment
    cmake -D GLFW_BUILD_EXAMPLES=OFF -D GLFW_BUILD_TESTS=OFF -D GLFW_BUILD_DOCS=OFF -D GLFW_BUILD_WAYLAND=OFF ..
    make -j$(nproc)
    cd ../../../
else
    echo "GLFW already exists."
fi

# 5. ONNX Runtime (Downloading binaries for Linux x64)
if [ ! -d "$EXTERNAL_DIR/onnxruntime" ]; then
    echo "Downloading ONNX Runtime..."
    ONNX_VER="1.16.3"
    curl -L https://github.com/microsoft/onnxruntime/releases/download/v${ONNX_VER}/onnxruntime-linux-x64-${ONNX_VER}.tgz -o "$EXTERNAL_DIR/onnxruntime.tgz"
    mkdir -p "$EXTERNAL_DIR/onnxruntime"
    tar -xzf "$EXTERNAL_DIR/onnxruntime.tgz" -C "$EXTERNAL_DIR/onnxruntime" --strip-components=1
    rm "$EXTERNAL_DIR/onnxruntime.tgz"
    echo "ONNX Runtime downloaded."
else
    echo "ONNX Runtime already exists."
fi

echo "Dependency setup complete."
