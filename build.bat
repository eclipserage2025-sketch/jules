@echo off
setlocal
title AI Crypto Miner Pro v3.1 Builder

echo ---------------------------------------------------------
echo AI Crypto Miner Pro v3.1: Ultimate Neural Edition
echo Automated Build Pipeline for Windows 10/11
echo ---------------------------------------------------------

:: Check for CMake
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found. Please install CMake and add it to your PATH.
    pause
    goto :eof
)

:: Create Build Directory
if not exist build_win64 mkdir build_win64

echo [1/3] Configuring Project...
cmake -B build_win64 -G "Visual Studio 17 2022" -A x64
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed. Ensure Qt6, OpenSSL and ONNX are reachable.
    pause
    goto :eof
)

echo [2/3] Compiling Source Code (Release)...
cmake --build build_win64 --config Release --parallel
if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed.
    pause
    goto :eof
)

echo [3/3] Deploying to ./release_dist...
if not exist release_dist mkdir release_dist
copy /y build_win64\Release\AIMiner.exe release_dist\AIMiner.exe
xcopy /s /i /y kernels release_dist\kernels

echo ---------------------------------------------------------
echo BUILD SUCCESSFUL!
echo Your miner is ready at: ./release_dist/AIMiner.exe
echo ---------------------------------------------------------
pause
