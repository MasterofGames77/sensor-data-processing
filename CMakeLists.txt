cmake_minimum_required(VERSION 3.12)
project(SensorDataProcessing VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(BUILD_TESTS "Build unit tests" ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/SensorReading.cpp
    src/SensorDataProcessor.cpp
    src/DataIngester.cpp
)

# Create executable
add_executable(sensor-processor ${SOURCES})

# Compiler flags for performance awareness (compiler-specific)
if(MSVC)
    # MSVC compiler flags
    target_compile_options(sensor-processor PRIVATE
        /W4           # Warning level 4
        /WX-          # Don't treat warnings as errors
        /O2           # Optimize for speed
    )
else()
    # GCC/Clang compiler flags
    target_compile_options(sensor-processor PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -O2
    )
endif()

# Unit tests
if(BUILD_TESTS)
    enable_testing()
    
    # Find or include a simple test framework
    # For simplicity, we'll create a basic test runner
    add_executable(test-runner
        tests/test_main.cpp
        tests/test_SensorReading.cpp
        tests/test_SensorDataProcessor.cpp
        src/SensorReading.cpp
        src/SensorDataProcessor.cpp
        src/DataIngester.cpp
    )
    
    target_include_directories(test-runner PRIVATE include)
    
    # Compiler flags for test runner (compiler-specific)
    if(MSVC)
        target_compile_options(test-runner PRIVATE /W4 /WX- /O2)
    else()
        target_compile_options(test-runner PRIVATE -Wall -Wextra -Wpedantic -O2)
    endif()
    
    add_test(NAME UnitTests COMMAND test-runner)
endif()

