#ifndef SENSOR_DATA_PROCESSOR_H
#define SENSOR_DATA_PROCESSOR_H

#include "SensorReading.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

/**
 * @brief Statistics structure for aggregated sensor data
 */
struct SensorStatistics {
    double min;
    double max;
    double mean;
    double median;
    size_t count;
    
    SensorStatistics() : min(0.0), max(0.0), mean(0.0), median(0.0), count(0) {}
};

/**
 * @brief Processes sensor data with filtering, aggregation, and transformation capabilities
 * 
 * This class provides core data processing functionality similar to what would be
 * found in defense system integration work: filtering, statistical analysis, and
 * data transformation.
 */
class SensorDataProcessor {
public:
    SensorDataProcessor();
    ~SensorDataProcessor() = default;

    /**
     * @brief Process a collection of sensor readings
     * @param readings Input sensor readings
     * @return Processed readings (may be filtered/transformed)
     */
    std::vector<SensorReading> process(const std::vector<SensorReading>& readings);

    /**
     * @brief Filter readings by sensor type
     * @param readings Input readings
     * @param type Sensor type to filter by
     * @return Filtered readings
     */
    std::vector<SensorReading> filterByType(
        const std::vector<SensorReading>& readings,
        SensorReading::SensorType type) const;

    /**
     * @brief Filter readings by sensor ID
     * @param readings Input readings
     * @param sensorId Sensor ID to filter by
     * @return Filtered readings
     */
    std::vector<SensorReading> filterBySensorId(
        const std::vector<SensorReading>& readings,
        const std::string& sensorId) const;

    /**
     * @brief Filter readings by value range
     * @param readings Input readings
     * @param minValue Minimum value threshold
     * @param maxValue Maximum value threshold
     * @return Filtered readings
     */
    std::vector<SensorReading> filterByValueRange(
        const std::vector<SensorReading>& readings,
        double minValue, double maxValue) const;

    /**
     * @brief Calculate statistics for a set of readings
     * @param readings Input readings
     * @return Statistics structure
     */
    SensorStatistics calculateStatistics(
        const std::vector<SensorReading>& readings) const;

    /**
     * @brief Calculate statistics grouped by sensor type
     * @param readings Input readings
     * @return Map of sensor type to statistics
     */
    std::map<SensorReading::SensorType, SensorStatistics> 
    calculateStatisticsByType(const std::vector<SensorReading>& readings) const;

    /**
     * @brief Calculate statistics grouped by sensor ID
     * @param readings Input readings
     * @return Map of sensor ID to statistics
     */
    std::map<std::string, SensorStatistics> 
    calculateStatisticsBySensorId(const std::vector<SensorReading>& readings) const;

    /**
     * @brief Remove outliers using IQR (Interquartile Range) method
     * @param readings Input readings
     * @return Readings with outliers removed
     */
    std::vector<SensorReading> removeOutliers(
        const std::vector<SensorReading>& readings) const;

    /**
     * @brief Normalize values to 0-1 range
     * @param readings Input readings (modified in place)
     */
    void normalizeValues(std::vector<SensorReading>& readings) const;

private:
    /**
     * @brief Calculate median value from sorted vector
     */
    double calculateMedian(std::vector<double>& values) const;

    /**
     * @brief Calculate quartiles for outlier detection
     */
    void calculateQuartiles(const std::vector<double>& sortedValues,
                           double& q1, double& q3) const;
};

#endif // SENSOR_DATA_PROCESSOR_H

