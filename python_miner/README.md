# AI Python Miner (Litecoin/Scrypt)

A functional, high-performance standalone Python miner for Litecoin, featuring AI-driven auto-tuning and difficulty forecasting.

## Features

- **High-Performance Hashing**: Uses `ltc_scrypt` (C-extension) for CPU mining and `PyOpenCL` for GPU mining.
- **AI Engine**: Integrated `scikit-learn` MLP model for hashrate prediction and Linear Regression for difficulty forecasting.
- **Stratum Protocol**: Custom JSON-RPC client with subscription, authorization, and share submission.
- **Multi-threaded**: Optimized for multi-core CPUs with dynamic thread management.
- **AI Auto-Tuning**: Automatically discovers the best intensity and thread settings for your hardware.

## Installation

### Prerequisites

- Python 3.8+
- OpenCL Drivers (AMD/NVIDIA/Intel) for GPU mining.
- C++ Build Tools (required for installing `ltc_scrypt`).

### Steps

1. **Clone the repository** (if you haven't already).
2. **Navigate to the project directory**:
   ```bash
   cd python_miner
   ```
3. **Install dependencies**:
   ```bash
   pip install -r requirements.txt
   ```
   *Note: On Windows, you might need to install `ltc_scrypt` from a pre-compiled wheel or ensure Visual Studio Build Tools are installed.*

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

Run the miner:
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
