# AI Monero Miner (RandomX) v1.0

A brand new, from-scratch C++ miner for Monero (XMR) featuring AI-driven thread optimization.

## Key Features

- **RandomX Mining Core**: Built on the official RandomX hashing algorithm for peak CPU efficiency.
- **AI Dynamic Threading**: An intelligent management engine that monitors real-time hashrate and adjusts thread counts to maximize performance based on hardware feedback.
- **Hardware-Aware Profiling**: Automatically detects L3 cache size and CPU topology to ensure the 2MB-per-thread RandomX requirement is met by default.
- **Lightweight CLI**: A robust command-line interface designed for performance and ease of monitoring.
- **Monero Stratum Support**: Custom JSON-RPC implementation for reliable pool connectivity.

## Project Structure

- `src/`: Core implementation files (`MiningCore`, `StratumClient`, `ThreadManager`, `SystemProfiler`).
- `include/`: Header files defining the project architecture.
- `scripts/`: Automation scripts for dependency management.
- `external/`: Third-party libraries (RandomX, nlohmann/json).
- `build/`: Compilation output directory.

## Getting Started

### 1. Build Dependencies
The miner requires `libcurl` and `RandomX`. Use the provided script to automate the setup:

```bash
chmod +x scripts/setup_dependencies.sh
./scripts/setup_dependencies.sh
```

*Note: Ensure `cmake`, `git`, and `build-essential` are installed on your system.*

### 2. Compile the Miner
Use CMake to build the project:

```bash
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

### 3. Run the Miner
Execute the binary:

```bash
./bin/ai-monero-miner
```

## AI Logic: Dynamic Threading

The miner uses a "Hill Climbing" AI heuristic to find the optimal thread count.
1. **Initial Phase**: It profiles the CPU L3 cache and starts with 1 thread per 2MB of cache.
2. **Monitoring Phase**: It observes the hashrate over a sliding window.
3. **Optimization Phase**: If the hashrate improves or stays stable, it may test an additional thread. If a significant drop is detected (due to cache thrashing or thermal throttling), it automatically scales back.

---
*Developed by Jules.*
