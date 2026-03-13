@echo off
setlocal
echo Building...
mkdir build 2>nul
cd build
cmake ..
cmake --build . --config Release
if %ERRORLEVEL% neq 0 ( echo Failed! ) else ( echo Success! )
pause