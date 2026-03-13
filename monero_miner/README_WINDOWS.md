# AI Monero Miner Pro (RandomX) - Windows Guide

## Prerequisites
- **Visual Studio 2022** (C++ Desktop Workload)
- **CMake**
- **Git for Windows**

## Build Instructions (Windows CMD)

### 1. Setup Dependencies
Manually clone RandomX into `external/RandomX` or use a package manager like `vcpkg` for `libcurl`, `glfw`, and `onnxruntime`.

### 2. Automated Build
Run the provided batch file from a Developer Command Prompt:
```cmd
build.bat
```

The executable will be generated in `build\bin\Release\ai-monero-miner-pro.exe`.

## Performance Tips
- **Run as Administrator**: Required for MSR tweaks and Large Page allocation.
- **Lock Pages in Memory**: Ensure your user has this permission in Local Security Policy for a 20%+ hashrate boost.

---
*Developed by Jules.*
