cmake_minimum_required(VERSION 3.20)
project(AIMiner VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Dependencies
find_package(Qt6 REQUIRED COMPONENTS Widgets Network Gui)
find_package(OpenCL REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

# ONNX Runtime (Assuming standard installation or provided path)
# In a real Windows environment, this might be a find_path/find_library or a FetchContent
# For now, we'll define the include and library placeholders
set(ONNXRUNTIME_ROOT_DIR "" CACHE PATH "Path to ONNX Runtime root")
include_directories(${ONNXRUNTIME_ROOT_DIR}/include)
link_directories(${ONNXRUNTIME_ROOT_DIR}/lib)

# Include directories
include_directories(include)
include_directories(src)

# Source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"
)

# UI files
file(GLOB_RECURSE UI_FILES "src/gui/*.ui")

# Resources
set(RESOURCE_FILES "resources.qrc")

# Executable
qt_add_executable(AIMiner
    ${SOURCES}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

# Link Libraries
target_link_libraries(AIMiner PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::Gui
    OpenCL::OpenCL
    CURL::libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
    onnxruntime
)

# Windows specific configurations
if(WIN32)
    # Define Windows-specific flags or link libraries like ADL (AMD Display Library)
    # target_link_libraries(AIMiner PRIVATE adl_sdk)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Installation rules (Optional but good practice)
install(TARGETS AIMiner DESTINATION bin)

# Testing
enable_testing()
add_executable(test_scrypt src/tests/test_scrypt.cpp src/core/scrypt.cpp)
target_include_directories(test_scrypt PRIVATE src)
target_link_libraries(test_scrypt PRIVATE OpenSSL::Crypto)
add_test(NAME TestScrypt COMMAND test_scrypt)
