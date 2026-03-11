# install_scrypt.ps1
# This script automates the building of the ltc_scrypt C extension on Windows.

Write-Host "--- AI Python Miner: Scrypt Extension Installer (Windows) ---" -ForegroundColor Cyan

# 1. Check for Python
if (!(Get-Command python -ErrorAction SilentlyContinue)) {
    Write-Host "Error: Python not found! Please install Python 3.8+ and add it to your PATH." -ForegroundColor Red
    return
}

# 2. Check for setuptools
Write-Host "Checking for setuptools..."
python -c "import setuptools" 2>$null
if ($LASTEXITCODE -ne 0) {
    Write-Host "Installing setuptools..." -ForegroundColor Yellow
    python -m pip install setuptools
}

# 3. Navigate to C extension directory
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$CExtDir = Join-Path (Split-Path -Parent $ScriptDir) "src\c_ext"

if (Test-Path $CExtDir) {
    Set-Location $CExtDir
    Write-Host "Building Scrypt extension in $CExtDir..." -ForegroundColor Green

    # 4. Build extension
    python setup.py build_ext --inplace

    if ($LASTEXITCODE -eq 0) {
        Write-Host "Scrypt extension built successfully!" -ForegroundColor Green
        Write-Host "A platform-specific .pyd file should now be in src\c_ext."
    } else {
        Write-Host "Error: Build failed! Please ensure 'Visual Studio C++ Build Tools' are installed." -ForegroundColor Red
    }
} else {
    Write-Host "Error: Could not find C extension source directory at $CExtDir." -ForegroundColor Red
}

Write-Host "--- Installation Complete ---" -ForegroundColor Cyan
