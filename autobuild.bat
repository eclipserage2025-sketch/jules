@echo off
setlocal enabledelayedexpansion
title AI Crypto Miner - Ultimate Auto-Builder

echo =========================================================
echo   AI Crypto Miner Pro v3.1 - AUTOMATED BUILD SYSTEM
echo =========================================================

:: 1. Check for Git
git --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [SKIP] Git not found.
) else (
    echo [OK] Git detected.
)

:: 2. Check for CMake
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] CMake is required but not found in PATH.
    echo Please install CMake from https://cmake.org/download/
    pause
    goto :eof
)
echo [OK] CMake detected.

:: 3. Check for Visual Studio
set "VS_PATH="
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe" (
    set "VS_PATH=2022 Community"
) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\Common7\IDE\devenv.exe" (
    set "VS_PATH=2022 Professional"
) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\devenv.exe" (
    set "VS_PATH=2022 Enterprise"
)

if "%VS_PATH%"=="" (
    echo [WARNING] Visual Studio 2022 not found in default locations.
) else (
    echo [OK] Visual Studio %VS_PATH% detected.
)

echo [STEP 1/3] Generating Build Files...
cmake -B build -G "Visual Studio 17 2022" -A x64

if %errorlevel% neq 0 (
    echo [ERROR] Configuration failed.
    pause
    goto :eof
)

echo [STEP 2/3] Compiling Ultimate Miner...
cmake --build build --config Release --parallel
if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    pause
    goto :eof
)

echo [STEP 3/3] Finalizing Artifacts...
if not exist dist mkdir dist
copy /y build\Release\AIMiner.exe dist\AIMiner.exe >nul
if exist kernels xcopy /s /i /y kernels dist\kernels >nul

echo =========================================================
echo   BUILD COMPLETE: ./dist/AIMiner.exe
echo =========================================================
pause
