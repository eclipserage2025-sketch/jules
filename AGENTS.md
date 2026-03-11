# AI Crypto Miner Development Guidelines

## Project Overview
This is a ground-up implementation of a Litecoin (Scrypt) miner with AI-driven optimization, targeting Windows with Qt6 and OpenCL.

## Coding Standards
- Use **Qt6** for GUI components.
- Implement hashing algorithms (Scrypt) directly in C++ and OpenCL without external mining libraries.
- Use **libcurl** for Stratum protocol implementation.
- Integrate **ONNX Runtime** for AI model execution.
- Monitor hardware via **AMD Display Library (ADL)** on Windows.

## Build System
- Use **CMake** for build configuration.
- Target C++17 or higher.

## Verification
- Run `test_minimal` to verify Scrypt hashing logic before submission.
