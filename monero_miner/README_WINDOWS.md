# AI Monero Miner (RandomX) - Windows Build Guide

This guide provides instructions for building and running the AI Monero Miner on Windows 10/11.

## Prerequisites

- **Visual Studio 2022**: Install with the "Desktop development with C++" workload.
- **CMake**: Download and install from [cmake.org](https://cmake.org/download/).
- **Git for Windows**: To clone the repository and dependencies.
- **Libcurl**: Recommended to use [vcpkg](https://github.com/microsoft/vcpkg) or download pre-built binaries.

## 1. Setup Dependencies

### Manual Setup
1. Clone the RandomX repository into `external/`:
   ```powershell
   cd external
   git clone https://github.com/tevador/RandomX.git
   ```
2. Build RandomX using CMake:
   ```powershell
   cd RandomX
   mkdir build; cd build
   cmake .. -G "Visual Studio 17 2022" -A x64
   cmake --build . --config Release
   ```

### Libcurl Setup (via vcpkg)
```powershell
vcpkg install curl:x64-windows
```

## 2. Compile the Miner

1. Open a Developer Command Prompt for VS 2022.
2. Navigate to the `monero_miner` directory.
3. Run the following:
   ```powershell
   mkdir build; cd build
   cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build . --config Release
   ```

## 3. Running

The executable will be located in `build/bin/Release/ai-monero-miner.exe`.

```powershell
./bin/Release/ai-monero-miner.exe
```

## Troubleshooting Windows-Specific Issues

### Large Pages
RandomX performs significantly better with "Large Pages" enabled.
1. Run the miner as Administrator.
2. The miner will attempt to lock memory. If it fails, ensure your user account has the "Lock pages in memory" permission in Windows Local Security Policy.

### WinRing0 (Optional)
For advanced hardware monitoring (CPU temperature), the miner may require administrative privileges to access MSR registers.

---
*Developed by Jules.*
