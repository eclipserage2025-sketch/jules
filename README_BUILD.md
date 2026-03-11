# Building AI Crypto Miner Pro v3.0

This document provides detailed instructions for compiling the AI Crypto Miner on a Windows development environment.

## 1. Prerequisites

### Tools
- **Visual Studio 2022** (with "Desktop development with C++" workload)
- **CMake 3.20+**
- **Git**

### Libraries & SDKs
- **Qt6 SDK**: Install via the Qt Online Installer. Required modules: `qtbase`, `qtnetwork`, `qtwidgets`.
- **OpenCL SDK**: Standard for AMD GPUs (part of ROCm or AMD GPU drivers).
- **OpenSSL**: Win64 OpenSSL v3.x recommended.
- **libcurl**: Can be built from source or installed via `vcpkg`.
- **ONNX Runtime**: Download the pre-built Windows x64 binaries from the official GitHub repository.

## 2. Environment Setup

Set the following environment variables or CMake hints to help the build system find your libraries:

```bash
set Qt6_DIR=C:/Qt/6.x.x/msvc2022_64/lib/cmake/Qt6
set ONNXRUNTIME_ROOT_DIR=C:/path/to/onnxruntime-win-x64-v1.x.x
```

## 3. Compilation Steps

### Using Command Line (PowerShell/CMD)

1. **Create Build Directory**:
   ```bash
   mkdir build
   cd build
   ```

2. **Configure with CMake**:
   ```bash
   cmake -G "Visual Studio 17 2022" -A x64 ..
   ```

3. **Build the Project**:
   ```bash
   cmake --build . --config Release
   ```

### Using Visual Studio IDE

1. Open Visual Studio.
2. Select **"Open a local folder"** and choose the project root.
3. Visual Studio will automatically detect `CMakeLists.txt`.
4. Wait for the CMake cache generation to finish.
5. Select the `AIMiner.exe` target and click **Build**.

## 4. Post-Build Setup

1. **DLLs**: Ensure all required DLLs (Qt6, OpenSSL, libcurl, onnxruntime.dll) are in the same folder as `AIMiner.exe` or in your system PATH.
2. **Kernels**: The `kernels/` directory must be in the same directory as the executable or reachable via the application's working directory so the OpenCL runtime can load `scrypt.cl`.

## 5. Running Tests

To verify the build:
1. Run `ctest` from the build directory.
2. Alternatively, run the generated `test_scrypt.exe` to verify the hashing core manually.

---
*Support: Contact the development team for any build-related issues.*
