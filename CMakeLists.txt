cmake_minimum_required(VERSION 3.24)
project(AIMiner VERSION 3.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization for Windows
if(MSVC)
    add_compile_options(/W4 /WX- /O2 /Oi /Gy /arch:AVX2)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX)
endif()

# -----------------------------------------------------------------------------
# Dependency Management (Automatic FetchContent)
# -----------------------------------------------------------------------------
include(FetchContent)

# 1. nlohmann_json (Automated Fetch)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# 2. libcurl (Automated Fetch)
FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG v8.6.0
)
set(BUILD_CURL_EXE OFF CACHE INTERNAL "")
set(BUILD_LIBCURL_DOCS OFF CACHE INTERNAL "")
set(CURL_ENABLE_SSL ON CACHE INTERNAL "")
FetchContent_MakeAvailable(curl)

# 3. ONNX Runtime (Automated Binary Fetch for Windows)
set(ONNX_VER "1.17.1")
if(WIN32)
    FetchContent_Declare(
        onnxruntime
        URL "https://github.com/microsoft/onnxruntime/releases/download/v${ONNX_VER}/onnxruntime-win-x64-${ONNX_VER}.zip"
    )
    FetchContent_MakeAvailable(onnxruntime)
endif()

# 4. Qt6 (Must be pre-installed via Qt Online Installer)
find_package(Qt6 REQUIRED COMPONENTS Widgets Network Gui Core)

# 5. OpenSSL & OpenCL (Standard System dependencies)
find_package(OpenSSL REQUIRED)
find_package(OpenCL REQUIRED)

# -----------------------------------------------------------------------------
# Project Configuration
# -----------------------------------------------------------------------------
include_directories(src)
include_directories(include)

# Gather all source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"
)

# Qt6 Resource system
set(RESOURCE_FILES "resources.qrc")

# -----------------------------------------------------------------------------
# Target: AIMiner (Windows GUI)
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
    nlohmann_json::nlohmann_json
)

# Link ONNX Runtime on Windows
if(WIN32)
    # The zip contains lib/onnxruntime.lib
    target_link_libraries(AIMiner PRIVATE "${onnxruntime_SOURCE_DIR}/lib/onnxruntime.lib")
    target_include_directories(AIMiner PRIVATE "${onnxruntime_SOURCE_DIR}/include")

    # Auto-copy DLL to output directory
    add_custom_command(TARGET AIMiner POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${onnxruntime_SOURCE_DIR}/lib/onnxruntime.dll"
        $<TARGET_FILE_DIR:AIMiner>
    )
endif()

# -----------------------------------------------------------------------------
# Unit Testing
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
# Deployment Logic
# -----------------------------------------------------------------------------
install(TARGETS AIMiner DESTINATION bin)
