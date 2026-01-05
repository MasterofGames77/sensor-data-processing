#!/bin/bash

# Build script for Sensor Data Processing project

set -e  # Exit on error

echo "Building Sensor Data Processing Service..."

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake ..

# Build the project
echo "Building project..."
make

echo ""
echo "Build complete!"
echo ""
echo "To run the application:"
echo "  ./sensor-processor -f ../data/sensor_data.csv -s"
echo ""
echo "To run tests:"
echo "  ./test-runner"

