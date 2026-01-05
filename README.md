# C++ Sensor Data Processing Service

A command-line C++ application that ingests simulated sensor data, processes it with filtering and statistical analysis, and outputs structured results. This project demonstrates systems thinking, performance awareness, and modular design suitable for defense/undersea system integration work.

## Features

- **Data Ingestion**: Read sensor data from CSV files or generate simulated data
- **Data Processing**: Filter, aggregate, and transform sensor readings
- **Statistical Analysis**: Calculate min, max, mean, median, and grouped statistics
- **Outlier Detection**: Remove outliers using IQR (Interquartile Range) method
- **Modular Design**: Clean separation of concerns with dedicated classes for data models, processing, and I/O
- **Unit Tests**: Comprehensive test coverage for core functionality

## Project Structure

```
sensor-data-processing/
├── CMakeLists.txt          # CMake build configuration
├── README.md               # This file
├── include/                # Header files
│   ├── SensorReading.h
│   ├── SensorDataProcessor.h
│   └── DataIngester.h
├── src/                    # Source files
│   ├── main.cpp
│   ├── SensorReading.cpp
│   ├── SensorDataProcessor.cpp
│   └── DataIngester.cpp
├── tests/                  # Unit tests
│   ├── test_main.cpp
│   ├── test_SensorReading.cpp
│   └── test_SensorDataProcessor.cpp
└── data/                   # Sample data files
    └── sensor_data.csv
```

## Requirements

- CMake 3.12 or higher
- C++17 compatible compiler (GCC, Clang, or MSVC)
- Linux environment (or WSL for Windows)

## Building the Project

### Linux/WSL

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
make

# Build with tests enabled (default)
cmake -DBUILD_TESTS=ON ..
make
```

### Windows (with Visual Studio)

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

## Running the Application

### Basic Usage

```bash
# Read from CSV file
./sensor-processor -f ../data/sensor_data.csv

# Generate simulated data and show statistics
./sensor-processor -g 1000 -s

# Process file and write output
./sensor-processor -f ../data/sensor_data.csv -o output.csv -s

# Show help
./sensor-processor -h
```

### Command Line Options

- `-f, --file <path>`: Read sensor data from CSV file
- `-g, --generate <num>`: Generate `<num>` simulated sensor readings
- `-o, --output <path>`: Write processed results to file
- `-s, --stats`: Show detailed statistics
- `-h, --help`: Show help message

## Running Tests

```bash
cd build
./test-runner
```

Or use CMake's test runner:

```bash
cd build
ctest
```

## CSV File Format

The CSV file should have the following format:

```csv
sensor_id,type,value,timestamp
SENSOR_001,TEMPERATURE,22.5,1704067200000
SENSOR_002,PRESSURE,1013.25,1704067201000
```

- **sensor_id**: Unique identifier for the sensor
- **type**: Sensor type (TEMPERATURE, PRESSURE, DEPTH, SONAR, ACCELEROMETER, GYROSCOPE)
- **value**: Measurement value (double)
- **timestamp**: Unix timestamp in milliseconds (int64)

## Design Decisions

### Performance Considerations

- Use of STL containers (`std::vector`, `std::map`) for efficient data structures
- Reserve capacity where possible to minimize reallocations
- Efficient algorithms for filtering and statistical calculations
- Compiler optimizations enabled (`-O2`)

### Modularity

- **SensorReading**: Encapsulates a single sensor reading with validation
- **SensorDataProcessor**: Handles all data processing operations
- **DataIngester**: Manages I/O operations (file reading/writing, data generation)
- Clear separation allows for easy extension and testing

### Code Quality

- Comprehensive documentation with Doxygen-style comments
- Input validation and error handling
- Unit tests for core functionality
- Consistent coding style and naming conventions

## Example Output

```
Reading sensor data from: data/sensor_data.csv
Loaded 10 sensor readings

Processing sensor data...
Processed 10 readings (removed 0 outliers/invalid)

Overall Statistics:
  Count:  10
  Min:    22.50
  Max:    5000.00
  Mean:   730.11
  Median: 152.30

Statistics by Sensor Type:
TEMPERATURE:
  Count:  4
  Min:    22.50
  Max:    25.80
  Mean:   23.90
  Median: 23.65
```

## Future Enhancements

- Real-time data streaming support
- Additional sensor types and processing algorithms
- Configuration file support for processing parameters
- Performance benchmarking and profiling
- Integration with external data sources

## License

This project is created for portfolio/resume purposes.
