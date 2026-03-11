# AI Python Miner (Litecoin/Scrypt)

A functional, high-performance standalone Python miner for Litecoin, featuring AI-driven auto-tuning and difficulty forecasting.

## Features

- **High-Performance Hashing**: Uses `ltc_scrypt` (C-extension) for CPU mining and `PyOpenCL` for GPU mining.
- **AI Engine**: Integrated `scikit-learn` MLP model for hashrate prediction and Linear Regression for difficulty forecasting.
- **Stratum Protocol**: Custom JSON-RPC client with subscription, authorization, and share submission.
- **Multi-threaded**: Optimized for multi-core CPUs with dynamic thread management.
- **AI Auto-Tuning**: Automatically discovers the best intensity and thread settings for your hardware.

## Installation Guide

### Prerequisites
- Python 3.8+
- `pip` (Python package manager)

---

### 🐧 Linux (Ubuntu/Debian)

1. **Install Build Tools and Dependencies**:
   ```bash
   sudo apt-get update
   sudo apt-get install build-essential python3-dev libssl-dev
   ```

2. **OpenCL Support** (Optional for GPU mining):
   - For AMD: `sudo apt-get install amdgpu-pro`
   - For NVIDIA: `sudo apt-get install nvidia-cuda-toolkit`

3. **Install Python Packages**:
   ```bash
   pip install -r requirements.txt
   ```

---

### 🍎 macOS (Intel/Apple Silicon)

1. **Install Command Line Tools**:
   ```bash
   xcode-select --install
   ```

2. **Install OpenCL Support**:
   - macOS has built-in OpenCL support, but ensure you are using a compatible Python version (Homebrew Python recommended).

3. **Install Python Packages**:
   ```bash
   pip install -r requirements.txt
   ```

---

### 🪟 Windows

1. **Install Build Tools**:
   - Download and install [Visual Studio Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/).
   - Ensure "Desktop development with C++" is selected during installation.

2. **OpenCL Support** (Optional for GPU mining):
   - Install the latest drivers for your GPU (NVIDIA/AMD/Intel).

3. **Install Python Packages**:
   ```powershell
   pip install -r requirements.txt
   ```
   *Note: If `ltc_scrypt` fails to compile, you may need to install a pre-compiled wheel or ensure the `vcvarsall.bat` environment is set up.*

---

## Configuration

Edit `config.json` to set your pool and wallet:

```json
{
    "pool_url": "stratum+tcp://litecoinpool.org:3333",
    "wallet_address": "LYourWalletAddressHere",
    "thread_count": 4,
    "ai_sensitivity": 0.5,
    "gpu_enabled": true
}
```

## Usage

Run the miner from the `python_miner` directory:
```bash
python main.py
```

The miner will automatically start, connect to the pool, and begin the AI auto-tuning process. Logs are saved to `miner.log` and displayed in the console.

## Project Structure

- `main.py`: Entry point, coordinates components.
- `src/ai_engine.py`: Machine learning logic for optimization.
- `src/mining_core.py`: Hashing implementation (CPU/GPU).
- `src/stratum_client.py`: Pool communication logic.
- `config.json`: Miner configuration.

---
*Developed by Jules.*

## Building the Hashing Extension (Optional)

If `ltc_scrypt` cannot be installed via pip, you can build the local C extension:

```bash
cd src/c_ext
python setup.py build_ext --inplace
```
This will generate a platform-specific binary (e.g., `.so` or `.pyd`) that the miner will automatically detect and use for high-performance hashing.
