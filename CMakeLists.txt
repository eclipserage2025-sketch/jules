cmake_minimum_required(VERSION 3.20)
project(AIMiner VERSION 3.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows Specific: GUI Application Subsystem
if(WIN32)
    set(GUI_TYPE WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-DNOMINMAX) # Fix for Windows.h and std::min/max conflict
else()
    set(GUI_TYPE "")
endif()

# 1. Find Qt6 (Standard on Windows via Qt Online Installer)
find_package(Qt6 REQUIRED COMPONENTS Widgets Network Gui Core)

# 2. Find OpenCL (AMD ROCm or GPU Driver SDK)
find_package(OpenCL REQUIRED)

# 3. Find OpenSSL (Common on Windows via Win64OpenSSL)
if(WIN32)
    set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64" CACHE PATH "Path to OpenSSL root")
endif()
find_package(OpenSSL REQUIRED)

# 4. Find libcurl (Often installed via vcpkg or manual build)
if(WIN32)
    # Hint for common curl installations
    set(CURL_LIBRARY_DIRS "C:/libcurl/lib" CACHE PATH "Path to libcurl lib")
    set(CURL_INCLUDE_DIRS "C:/libcurl/include" CACHE PATH "Path to libcurl include")
endif()
find_package(CURL REQUIRED)

# 5. ONNX Runtime (Manual configuration for Windows)
set(ONNXRUNTIME_ROOT_DIR "C:/onnxruntime" CACHE PATH "Path to ONNX Runtime root (Windows x64)")
include_directories(${ONNXRUNTIME_ROOT_DIR}/include)
link_directories(${ONNXRUNTIME_ROOT_DIR}/lib)

# Include Project Directories
include_directories(src)
include_directories(include)

# Gather Source Files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"
)

# Resources (Qt6 RCC)
set(RESOURCE_FILES "resources.qrc")

# Main Executable
qt_add_executable(AIMiner ${GUI_TYPE}
    ${SOURCES}
    ${RESOURCE_FILES}
)

# Linkage
target_link_libraries(AIMiner PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::Gui
    Qt6::Core
    OpenCL::OpenCL
    CURL::libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
    onnxruntime
)

# Windows ADL SDK (AMD Display Library)
# In a production Windows environment, the user would provide the path to the ADL SDK
if(WIN32)
    # target_include_directories(AIMiner PRIVATE "C:/AMD/ADL_SDK/include")
    # target_link_libraries(AIMiner PRIVATE gdi32 user32 shell32)
endif()

# Installation rules
install(TARGETS AIMiner DESTINATION bin)

# ---------------------------------------------------------
# Unit Tests (Cross-platform)
# ---------------------------------------------------------
enable_testing()

add_executable(test_scrypt src/tests/test_scrypt.cpp src/core/scrypt.cpp)
target_include_directories(test_scrypt PRIVATE src)
target_link_libraries(test_scrypt PRIVATE OpenSSL::Crypto)

add_executable(test_neural src/tests/test_neural_net.cpp src/ai/neural_net.cpp)
target_include_directories(test_neural PRIVATE src)

add_test(NAME TestScrypt COMMAND test_scrypt)
add_test(NAME TestNeural COMMAND test_neural)
