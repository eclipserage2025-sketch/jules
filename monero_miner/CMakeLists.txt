cmake_minimum_required(VERSION 3.10)
project(AI-Monero-Miner VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Includes
include_directories(include)
include_directories(external)
include_directories(external/RandomX/src)

# Libraries
# find_package(CURL REQUIRED) # Disabled for environments without libcurl-dev
set(RANDOMX_LIB "${CMAKE_CURRENT_SOURCE_DIR}/external/RandomX/build/librandomx.a")

# Source Files
set(SOURCES
    src/main.cpp
    src/SystemProfiler.cpp
    src/MiningCore.cpp
    src/StratumClient.cpp
    src/ThreadManager.cpp
)

add_executable(ai-monero-miner ${SOURCES})

# Linking
target_link_libraries(ai-monero-miner
    # ${CURL_LIBRARIES}
    ${RANDOMX_LIB}
    pthread
)

# Output directory
set_target_properties(ai-monero-miner PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
