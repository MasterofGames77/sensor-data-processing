#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "SensorReading.h"
#include "SensorDataProcessor.h"
#include "DataIngester.h"

/**
 * @brief Print usage information
 */
void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [OPTIONS]\n"
              << "Options:\n"
              << "  -f, --file <path>     Read sensor data from CSV file\n"
              << "  -g, --generate <num>   Generate <num> simulated sensor readings\n"
              << "  -o, --output <path>    Write processed results to file\n"
              << "  -s, --stats            Show detailed statistics\n"
              << "  -h, --help             Show this help message\n"
              << "\n"
              << "Examples:\n"
              << "  " << programName << " -f data/sensor_data.csv -s\n"
              << "  " << programName << " -g 1000 -o output.csv -s\n";
}

/**
 * @brief Print sensor reading in formatted way
 */
void printReading(const SensorReading& reading) {
    std::cout << std::fixed << std::setprecision(2)
              << "[" << reading.getSensorId() << "] "
              << std::setw(15) << std::left 
              << SensorReading::typeToString(reading.getType())
              << " Value: " << std::setw(10) << reading.getValue()
              << " Timestamp: " << reading.getTimestamp() << "\n";
}

/**
 * @brief Print statistics in formatted way
 */
void printStatistics(const SensorStatistics& stats, const std::string& label = "") {
    std::cout << "\n" << (label.empty() ? "Statistics" : label) << ":\n"
              << "  Count:  " << stats.count << "\n"
              << "  Min:    " << std::fixed << std::setprecision(2) << stats.min << "\n"
              << "  Max:    " << stats.max << "\n"
              << "  Mean:   " << stats.mean << "\n"
              << "  Median: " << stats.median << "\n";
}

int main(int argc, char* argv[]) {
    std::string inputFile;
    std::string outputFile;
    size_t generateCount = 0;
    bool showStats = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-f" || arg == "--file") {
            if (i + 1 < argc) {
                inputFile = argv[++i];
            } else {
                std::cerr << "Error: -f requires a file path\n";
                return 1;
            }
        } else if (arg == "-g" || arg == "--generate") {
            if (i + 1 < argc) {
                generateCount = std::stoul(argv[++i]);
            } else {
                std::cerr << "Error: -g requires a count\n";
                return 1;
            }
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                outputFile = argv[++i];
            } else {
                std::cerr << "Error: -o requires a file path\n";
                return 1;
            }
        } else if (arg == "-s" || arg == "--stats") {
            showStats = true;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    DataIngester ingester;
    SensorDataProcessor processor;
    std::vector<SensorReading> readings;

    try {
        // Ingest data
        if (!inputFile.empty()) {
            std::cout << "Reading sensor data from: " << inputFile << "\n";
            readings = ingester.readFromFile(inputFile);
            std::cout << "Loaded " << readings.size() << " sensor readings\n";
        } else if (generateCount > 0) {
            std::cout << "Generating " << generateCount << " simulated sensor readings...\n";
            std::vector<std::string> sensorIds = {
                "SENSOR_001", "SENSOR_002", "SENSOR_003", "SENSOR_004"
            };
            std::vector<SensorReading::SensorType> types = {
                SensorReading::SensorType::TEMPERATURE,
                SensorReading::SensorType::PRESSURE,
                SensorReading::SensorType::DEPTH,
                SensorReading::SensorType::SONAR
            };
            readings = ingester.generateSimulatedData(generateCount, sensorIds, types);
            std::cout << "Generated " << readings.size() << " sensor readings\n";
        } else {
            std::cerr << "Error: Must specify either -f or -g option\n";
            printUsage(argv[0]);
            return 1;
        }

        if (readings.empty()) {
            std::cerr << "Error: No sensor readings to process\n";
            return 1;
        }

        // Process data
        std::cout << "\nProcessing sensor data...\n";
        std::vector<SensorReading> processed = processor.process(readings);
        std::cout << "Processed " << processed.size() << " readings "
                  << "(removed " << (readings.size() - processed.size()) << " outliers/invalid)\n";

        // Display statistics if requested
        if (showStats) {
            SensorStatistics overallStats = processor.calculateStatistics(processed);
            printStatistics(overallStats, "Overall Statistics");

            // Statistics by type
            auto statsByType = processor.calculateStatisticsByType(processed);
            if (!statsByType.empty()) {
                std::cout << "\nStatistics by Sensor Type:\n";
                for (const auto& pair : statsByType) {
                    std::string typeStr = SensorReading::typeToString(pair.first);
                    printStatistics(pair.second, typeStr);
                }
            }

            // Statistics by sensor ID
            auto statsBySensor = processor.calculateStatisticsBySensorId(processed);
            if (!statsBySensor.empty()) {
                std::cout << "\nStatistics by Sensor ID:\n";
                for (const auto& pair : statsBySensor) {
                    printStatistics(pair.second, pair.first);
                }
            }
        }

        // Write output if specified
        if (!outputFile.empty()) {
            if (ingester.writeToFile(processed, outputFile)) {
                std::cout << "\nProcessed data written to: " << outputFile << "\n";
            } else {
                std::cerr << "Error: Failed to write output file\n";
                return 1;
            }
        } else {
            // Print first 10 readings as sample
            std::cout << "\nSample processed readings (first 10):\n";
            size_t printCount = std::min(static_cast<size_t>(10), processed.size());
            for (size_t i = 0; i < printCount; ++i) {
                printReading(processed[i]);
            }
            if (processed.size() > 10) {
                std::cout << "... (" << (processed.size() - 10) << " more readings)\n";
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

