#ifndef DATA_INGESTER_H
#define DATA_INGESTER_H

#include "SensorReading.h"
#include <vector>
#include <string>
#include <fstream>

/**
 * @brief Handles ingestion of sensor data from various sources
 * 
 * Supports reading from CSV files and generating simulated sensor data
 * for testing and demonstration purposes.
 */
class DataIngester {
public:
    DataIngester();
    ~DataIngester() = default;

    /**
     * @brief Read sensor readings from a CSV file
     * @param filepath Path to CSV file
     * @return Vector of sensor readings
     * @throws std::runtime_error if file cannot be opened or parsed
     */
    std::vector<SensorReading> readFromFile(const std::string& filepath);

    /**
     * @brief Generate simulated sensor data
     * @param count Number of readings to generate
     * @param sensorIds Vector of sensor IDs to use
     * @param types Vector of sensor types to use
     * @return Vector of simulated sensor readings
     */
    std::vector<SensorReading> generateSimulatedData(
        size_t count,
        const std::vector<std::string>& sensorIds,
        const std::vector<SensorReading::SensorType>& types);

    /**
     * @brief Write sensor readings to CSV file
     * @param readings Readings to write
     * @param filepath Output file path
     * @return true if successful, false otherwise
     */
    bool writeToFile(const std::vector<SensorReading>& readings,
                     const std::string& filepath) const;

private:
    /**
     * @brief Parse a single line from CSV file
     * @param line CSV line
     * @return SensorReading object
     */
    SensorReading parseCSVLine(const std::string& line);

    /**
     * @brief Get current timestamp in milliseconds
     */
    int64_t getCurrentTimestamp() const;

    /**
     * @brief Generate random value within range
     */
    double generateRandomValue(double min, double max) const;
};

#endif // DATA_INGESTER_H

