# Step-by-Step Build Instructions

This document provides detailed step-by-step instructions to build and run the Sensor Data Processing project.

## Prerequisites Check

1. **Check if CMake is installed:**

   ```bash
   cmake --version
   ```

   If not installed, install it:

   - Ubuntu/Debian: `sudo apt-get install cmake`
   - macOS: `brew install cmake`
   - Windows: Download from https://cmake.org/download/

2. **Check if you have a C++17 compiler:**
   ```bash
   g++ --version  # Linux/WSL
   clang++ --version  # macOS/Linux
   ```
   If not installed:
   - Ubuntu/Debian: `sudo apt-get install build-essential`
   - macOS: Install Xcode Command Line Tools: `xcode-select --install`

## Step 1: Navigate to Project Directory

```bash
cd /path/to/sensor-data-processing
```

## Step 2: Create Build Directory

```bash
mkdir build
cd build
```

## Step 3: Configure with CMake

```bash
cmake ..
```

This will:

- Detect your compiler
- Configure the build system
- Generate Makefiles (on Linux/macOS) or Visual Studio project files (on Windows)

Expected output should show:

```
-- The CXX compiler identification is ...
-- Configuring done
-- Generating done
-- Build files have been written to: ...
```

## Step 4: Build the Project

### Linux/macOS/WSL:

```bash
make
```

### Windows (using Visual Studio):

```bash
cmake --build . --config Release
```

This will compile:

- `sensor-processor` - The main application
- `test-runner` - The unit test executable (if BUILD_TESTS is ON)

## Step 5: Verify Build

You should see two executables in the `build` directory:

- `sensor-processor` (or `sensor-processor.exe` on Windows)
- `test-runner` (or `test-runner.exe` on Windows)

## Step 6: Run Tests

```bash
./test-runner
```

Expected output:

```
Running unit tests...

=== SensorReading Tests ===
  Default Constructor... PASS
  Parameterized Constructor... PASS
  ...
=== Test Summary ===
Tests run: 13
Tests passed: 13
Tests failed: 0

✓ All tests passed!
```

## Step 7: Run the Application

### Option A: Use Sample Data File

```bash
./sensor-processor -f ../data/sensor_data.csv -s
```

### Option B: Generate Simulated Data

```bash
./sensor-processor -g 1000 -s
```

### Option C: Process and Save Output

```bash
./sensor-processor -f ../data/sensor_data.csv -o output.csv -s
```

## Troubleshooting

### Issue: "CMake not found"

**Solution:** Install CMake (see Prerequisites)

### Issue: "No C++ compiler found"

**Solution:** Install a C++ compiler (see Prerequisites)

### Issue: "Cannot open file: ../data/sensor_data.csv"

**Solution:** Make sure you're running from the `build` directory, or use an absolute path

### Issue: Compilation errors

**Solution:**

1. Make sure you're using C++17 compatible compiler
2. Check that all source files are present
3. Try cleaning and rebuilding:
   ```bash
   cd build
   rm -rf *
   cmake ..
   make
   ```

### Issue: Tests fail

**Solution:**

1. Check that all source files compiled successfully
2. Verify you're running from the build directory
3. Check test output for specific failure messages

## Quick Build Script

For convenience, you can use the provided build script:

```bash
chmod +x build.sh
./build.sh
```

This will automatically:

1. Create the build directory
2. Configure with CMake
3. Build the project
4. Show usage instructions

## Next Steps

Once the project builds successfully:

1. Review the README.md for detailed usage
2. Experiment with different command-line options
3. Modify the code to add your own features
4. Add it to your resume/portfolio!
