# AI Crypto Miner Pro v3.1 - One-Click Build Script for Windows
# This script automates the CMake configuration and compilation process.

Write-Host "--- AI Crypto Miner Pro v3.1: Ultimate Neural Edition ---" -ForegroundColor Cyan
Write-Host "Starting Automated Build Pipeline..." -ForegroundColor Yellow

# 1. Check for Prerequisites
$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmake) {
    Write-Host "[ERROR] CMake not found. Please install CMake and add it to your PATH." -ForegroundColor Red
    return
}

# 2. Setup Build Environment
$BuildDir = "build_win64"
if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir
}

Write-Host "[1/3] Configuring Project with CMake..." -ForegroundColor Green
cmake -B $BuildDir -G "Visual Studio 17 2022" -A x64

if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] CMake configuration failed. Check your dependencies (Qt6, OpenSSL)." -ForegroundColor Red
    return
}

Write-Host "[2/3] Compiling Source Code (Release)..." -ForegroundColor Green
cmake --build $BuildDir --config Release --parallel

if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] Compilation failed." -ForegroundColor Red
    return
}

Write-Host "[3/3] Deploying Application Artifacts..." -ForegroundColor Green
$OutputDir = "release_dist"
if (-not (Test-Path $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir
}

if (Test-Path "$BuildDir/Release/AIMiner.exe") {
    Copy-Item "$BuildDir/Release/AIMiner.exe" -Destination "$OutputDir/AIMiner.exe"
}

if (Test-Path "kernels") {
    Copy-Item -Path "kernels" -Destination "$OutputDir/kernels" -Recurse -Force
}

Write-Host "--- BUILD SUCCESSFUL ---" -ForegroundColor Cyan
Write-Host "Executable is located in: ./$OutputDir/AIMiner.exe" -ForegroundColor White
Write-Host "Note: Ensure Qt6 and OpenSSL DLLs are in the folder or your PATH." -ForegroundColor Yellow
