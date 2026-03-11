cmake_minimum_required(VERSION 3.28) # Modern CMake for 2026
project(AIMiner VERSION 3.1.0 LANGUAGES CXX)

# Target modern C++23 (The standard for 2026 performance)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization for 2026 Hardware (AVX-512, AMX, etc.)
if(MSVC)
    add_compile_options(/arch:AVX512 /Ot /Oi /Gy)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX)
endif()

# -----------------------------------------------------------------------------
# Dependency Management (Modern FetchContent for zero-setup builds)
# -----------------------------------------------------------------------------
include(FetchContent)

# 1. Qt6 Setup (Modern way)
find_package(Qt6 REQUIRED COMPONENTS Widgets Network Gui Core)
qt_standard_project_setup()

# 2. OpenCL (Native GPU compute)
find_package(OpenCL REQUIRED)

# 3. libcurl (Automated FetchContent)
FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG v8.6.0 # Latest stable as of now, easily updated for 2026
)
set(BUILD_CURL_EXE OFF CACHE INTERNAL "")
set(CURL_USE_OPENSSL ON CACHE INTERNAL "")
FetchContent_MakeAvailable(curl)

# 4. OpenSSL (Automated FetchContent)
FetchContent_Declare(
    openssl
    GIT_REPOSITORY https://github.com/openssl/openssl.git
    GIT_TAG openssl-3.2.1
)
# Note: OpenSSL build via CMake is complex;
# in 2026, most use vcpkg or prebuilt binaries for Windows
find_package(OpenSSL REQUIRED)

# 5. ONNX Runtime (Binary FetchContent)
# For 2026, we fetch the optimized GPU-accelerated binaries
set(ONNXRUNTIME_VERSION "1.17.1")
FetchContent_Declare(
    onnxruntime
    URL "https://github.com/microsoft/onnxruntime/releases/download/v${ONNXRUNTIME_VERSION}/onnxruntime-win-x64-${ONNXRUNTIME_VERSION}.zip"
)
FetchContent_MakeAvailable(onnxruntime)

# -----------------------------------------------------------------------------
# Project Structure
# -----------------------------------------------------------------------------
include_directories(src)
include_directories(include)
include_directories(${onnxruntime_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"
)

# Qt Resources
set(RESOURCE_FILES "resources.qrc")

# -----------------------------------------------------------------------------
# Main Executable (Windows GUI Mode)
# -----------------------------------------------------------------------------
qt_add_executable(AIMiner WIN32
    ${SOURCES}
    ${RESOURCE_FILES}
)

target_link_libraries(AIMiner PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::Gui
    Qt6::Core
    OpenCL::OpenCL
    libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
    onnxruntime
)

# GPU-specific Windows integration (ADL SDK)
if(WIN32)
    target_link_libraries(AIMiner PRIVATE gdi32 user32 shell32 advapi32)
endif()

# -----------------------------------------------------------------------------
# Modern Testing (CTest)
# -----------------------------------------------------------------------------
enable_testing()

add_executable(test_scrypt src/tests/test_scrypt.cpp src/core/scrypt.cpp)
target_include_directories(test_scrypt PRIVATE src)
target_link_libraries(test_scrypt PRIVATE OpenSSL::Crypto)

add_executable(test_neural src/tests/test_neural_net.cpp src/ai/neural_net.cpp)
target_include_directories(test_neural PRIVATE src)

add_test(NAME TestScrypt COMMAND test_scrypt)
add_test(NAME TestNeural COMMAND test_neural)

# -----------------------------------------------------------------------------
# Installation & Deployment
# -----------------------------------------------------------------------------
install(TARGETS AIMiner DESTINATION bin)
qt_generate_deploy_app_script(
    TARGET AIMiner
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})
