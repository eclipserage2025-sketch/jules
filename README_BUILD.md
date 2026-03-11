# Building AI Crypto Miner Pro v3.1 (Future-Proof Edition 2026)

This project uses modern CMake 3.28+ features to automate dependency management and target high-performance 2026 hardware.

## 1. Prerequisites (Windows 2026)

### Tools
- **Visual Studio 2022 or 2025** (with "Desktop development with C++" and C++23/26 support)
- **CMake 3.28+**
- **Git**

### Libraries (Automated)
Most dependencies are automatically managed via **CMake FetchContent**:
- **libcurl**: Automatically fetched and built.
- **ONNX Runtime**: Automatically fetched from GitHub releases.
- **OpenSSL**: Should be pre-installed on the system (e.g., via `vcpkg install openssl:x64-windows`).
- **Qt6 SDK**: Installed via the Qt Online Installer.
- **OpenCL SDK**: Part of modern GPU drivers (AMD/NVIDIA).

## 2. Compilation Steps (Automated Build)

### Using Command Line (2026 Recommended)

1. **Configure the Project**:
   ```bash
   cmake -B build -G "Visual Studio 17 2022" -A x64
   ```
   *Note: This will automatically download libcurl and ONNX Runtime.*

2. **Build the Project**:
   ```bash
   cmake --build build --config Release --parallel
   ```

3. **Deploy (Automated)**:
   ```bash
   cmake --install build --prefix ./dist
   ```

## 3. Advanced Hardware Optimization
The 2026 build system is configured to target:
- **AVX-512** for ultra-fast Scrypt hashing on modern CPUs.
- **C++23 Standard** for superior memory management and concurrency.
- **GPU-Accelerated Inference**: The ONNX integration is ready for direct GPU execution providers.

---
*Developed by Jules. Built for the future.*
