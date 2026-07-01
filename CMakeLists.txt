cmake_minimum_required(VERSION 3.20)
project(cnc_simulator VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Third-party: nlohmann/json
include(FetchContent)
FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# Third-party: Google Test
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/v1.14.0.tar.gz)
FetchContent_MakeAvailable(googletest)

# Main executable
add_executable(cnc_simulator
    src/main.cpp
    src/parser/GcodeParser.cpp
    src/motion/MotionEngine.cpp
    src/simulator/Simulator.cpp
    src/logger/Logger.cpp
    src/motion/MachineConfig.cpp
)

target_include_directories(cnc_simulator PRIVATE include)

find_package(Threads REQUIRED)

target_link_libraries(cnc_simulator PRIVATE
    nlohmann_json::nlohmann_json
    Threads::Threads
)

# Tests
add_subdirectory(tests)
