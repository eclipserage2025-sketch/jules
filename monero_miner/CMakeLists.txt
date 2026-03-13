cmake_minimum_required(VERSION 3.10)
project(AI-Monero-Miner-Pro VERSION 2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# External Dirs
set(EXT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(IMGUI_DIR ${EXT_DIR}/imgui)
set(GLFW_DIR ${EXT_DIR}/glfw)
set(ONNX_DIR ${EXT_DIR}/onnxruntime)

# Includes
include_directories(include)
include_directories(external)
include_directories(external/RandomX/src)
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)
include_directories(${GLFW_DIR}/include)
include_directories(${ONNX_DIR}/include)

# Libraries
find_package(CURL REQUIRED)
find_package(OpenGL REQUIRED)

set(RANDOMX_LIB "${CMAKE_CURRENT_SOURCE_DIR}/external/RandomX/build/librandomx.a")
set(GLFW_LIB "${GLFW_DIR}/build/src/libglfw3.a")
set(ONNX_LIB "${ONNX_DIR}/lib/libonnxruntime.so")

# Source Files
set(SOURCES
    src/main.cpp
    src/SystemProfiler.cpp
    src/MiningCore.cpp
    src/StratumClient.cpp
    src/ThreadManager.cpp
    src/MinerGUI.cpp
    src/AIPredictor.cpp
    src/Optimizations.cpp
    # ImGui Backend Files
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(ai-monero-miner-pro ${SOURCES})

# Linking
target_link_libraries(ai-monero-miner-pro
    ${CURL_LIBRARIES}
    ${RANDOMX_LIB}
    ${GLFW_LIB}
    ${OPENGL_LIBRARIES}
    ${ONNX_LIB}
    dl
    pthread
)

# Deployment
set_target_properties(ai-monero-miner-pro PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
