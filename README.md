# AI Crypto Miner Pro v2.0 (Litecoin/Scrypt)

A high-performance, AI-driven cryptocurrency miner built from the ground up for Windows.

## Key Features

- **Ground-up Scrypt Engine**: Custom implementation of the Scrypt hashing algorithm in C++ (CPU) and OpenCL (AMD GPU).
- **AI Auto-Tuning**: Automatically discovers the optimal Intensity and Thread settings for your hardware.
- **AI Network Intelligence (Diffu)**: Forecasts network difficulty trends and rebalances strategy between Performance and Efficiency.
- **Profitability-Aware Switching**: Real-time switching between LTC and DOGE based on profitability scores.
- **Multi-Pool Failover**: Intelligent pool manager that handles backup connections with priority tracking.
- **Predictive Maintenance**: AI-driven thermal drift detection to protect your hardware from overheating and instability.
- **Qt6 Dashboard**: A professional Windows GUI for real-time monitoring and control.

## Build Requirements

- Windows 10/11
- CMake 3.20+
- Qt6 (Widgets, Gui, Network)
- OpenCL SDK (AMD/ROCm)
- OpenSSL & libcurl
- ONNX Runtime

## Getting Started

1. Open the project in Visual Studio or use CMake directly.
2. Configure the build with your library paths.
3. Build the `AIMiner` target.
4. Run the application and click "Start AI Miner".

## Testing

Run the included unit tests to verify the core hashing and AI logic:
- `test_scrypt`: Verifies the Scrypt core.
- (Internal suites for AI strategy and failover logic).

---
*Developed by Jules.*
