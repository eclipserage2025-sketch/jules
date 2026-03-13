# AI Monero Miner Pro (RandomX) v2.0

A high-performance, from-scratch C++ miner for Monero (XMR) featuring an ImGui dashboard and ONNX-driven AI optimization.

## Key Features

- **Modern Minimalist GUI**: A sleek Dear ImGui dashboard with real-time hashrate graphs.
- **ONNX AI Predictor**: Predictive hashrate forecasting using trained machine learning models.
- **RandomX Optimized**: Built on the official RandomX algorithm with support for Large Pages and MSR tweaks.
- **Secure Mining**: SSL/TLS support for encrypted communication with mining pools.
- **Hardware-Aware Profiling**: Automatic CPU and L3 cache detection for optimal configuration.

## Getting Started (Linux)

### 1. Build Dependencies
```bash
chmod +x scripts/setup_dependencies.sh
./scripts/setup_dependencies.sh
```

### 2. Compile and Run
We provide an automated script to handle the CMake build process:
```bash
chmod +x build.sh
./build.sh
./build/bin/ai-monero-miner-pro
```

## AI Logic: Dynamic Threading
The miner uses a combination of an ONNX-based predictive model and a real-time hill-climbing heuristic to find the hardware's optimal thread count, balancing performance and thermal stability.

---
*Developed by Jules.*
