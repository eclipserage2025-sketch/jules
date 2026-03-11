# build_exe.ps1
# Automates building the high-performance Scrypt extension and bundling the miner into a standalone Windows .exe.

Write-Host "--- AI Python Miner: Windows EXE Builder ---" -ForegroundColor Cyan

# 1. Check for Python
if (!(Get-Command python -ErrorAction SilentlyContinue)) {
    Write-Host "Error: Python not found! Please install Python 3.8+ and add it to your PATH." -ForegroundColor Red
    return
}

# 2. Install dependencies
Write-Host "Installing/Updating required build tools..." -ForegroundColor Yellow
python -m pip install pyinstaller setuptools numpy scikit-learn requests

# 3. Build the C extension first
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$ProjectDir = Split-Path -Parent $ScriptDir
$CExtDir = Join-Path $ProjectDir "src\c_ext"

Write-Host "Building Scrypt extension..." -ForegroundColor Yellow
Push-Location $CExtDir
python setup.py build_ext --inplace
if ($LASTEXITCODE -ne 0) {
    Write-Host "Error: Failed to build Scrypt extension! Please ensure MSVC build tools are installed." -ForegroundColor Red
    Pop-Location
    return
}
Pop-Location

# 4. Use PyInstaller to create the EXE
Write-Host "Packaging miner into standalone EXE..." -ForegroundColor Yellow
Set-Location $ProjectDir

# Find the built .pyd file
$PydFile = Get-ChildItem -Path "src\c_ext\*.pyd" | Select-Object -First 1
if (!$PydFile) {
    # Fallback to .so if on a non-Windows environment testing this
    $PydFile = Get-ChildItem -Path "src\c_ext\*.so" | Select-Object -First 1
}

if ($PydFile) {
    $PydPath = "src/c_ext/$($PydFile.Name);src/c_ext"
    Write-Host "Including built binary: $($PydFile.Name)" -ForegroundColor Green

    # Run PyInstaller
    # --onefile: Bundles everything into a single .exe
    # --add-data: Includes config.json and the built binary
    # --name: Resulting EXE name
    pyinstaller --onefile `
                --name "AI_Miner_Ultimate" `
                --add-data "config.json;." `
                --add-data "$PydPath" `
                main.py

    if ($LASTEXITCODE -eq 0) {
        Write-Host "Build Successful! Your standalone miner is in the 'dist' folder." -ForegroundColor Green
    } else {
        Write-Host "Error: PyInstaller failed." -ForegroundColor Red
    }
} else {
    Write-Host "Error: No built binary found to bundle!" -ForegroundColor Red
}

Write-Host "--- Build Process Complete ---" -ForegroundColor Cyan
