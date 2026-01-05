#include "SensorDataProcessor.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iterator>

SensorDataProcessor::SensorDataProcessor() {
}

std::vector<SensorReading> SensorDataProcessor::process(
    const std::vector<SensorReading>& readings) {
    
    // Basic processing pipeline: validate, filter invalid, remove outliers
    std::vector<SensorReading> validReadings;
    std::copy_if(readings.begin(), readings.end(),
                 std::back_inserter(validReadings),
                 [](const SensorReading& r) { return r.isValid(); });
    
    if (validReadings.empty()) {
        return validReadings;
    }
    
    // Remove outliers
    return removeOutliers(validReadings);
}

std::vector<SensorReading> SensorDataProcessor::filterByType(
    const std::vector<SensorReading>& readings,
    SensorReading::SensorType type) const {
    
    std::vector<SensorReading> filtered;
    std::copy_if(readings.begin(), readings.end(),
                 std::back_inserter(filtered),
                 [type](const SensorReading& r) { return r.getType() == type; });
    return filtered;
}

std::vector<SensorReading> SensorDataProcessor::filterBySensorId(
    const std::vector<SensorReading>& readings,
    const std::string& sensorId) const {
    
    std::vector<SensorReading> filtered;
    std::copy_if(readings.begin(), readings.end(),
                 std::back_inserter(filtered),
                 [&sensorId](const SensorReading& r) {
                     return r.getSensorId() == sensorId;
                 });
    return filtered;
}

std::vector<SensorReading> SensorDataProcessor::filterByValueRange(
    const std::vector<SensorReading>& readings,
    double minValue, double maxValue) const {
    
    std::vector<SensorReading> filtered;
    std::copy_if(readings.begin(), readings.end(),
                 std::back_inserter(filtered),
                 [minValue, maxValue](const SensorReading& r) {
                     double val = r.getValue();
                     return val >= minValue && val <= maxValue;
                 });
    return filtered;
}

SensorStatistics SensorDataProcessor::calculateStatistics(
    const std::vector<SensorReading>& readings) const {
    
    SensorStatistics stats;
    stats.count = readings.size();
    
    if (readings.empty()) {
        return stats;
    }
    
    std::vector<double> values;
    values.reserve(readings.size());
    for (const auto& reading : readings) {
        values.push_back(reading.getValue());
    }
    
    std::sort(values.begin(), values.end());
    
    stats.min = values.front();
    stats.max = values.back();
    
    // Calculate mean
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    stats.mean = sum / values.size();
    
    // Calculate median
    stats.median = calculateMedian(values);
    
    return stats;
}

std::map<SensorReading::SensorType, SensorStatistics> 
SensorDataProcessor::calculateStatisticsByType(
    const std::vector<SensorReading>& readings) const {
    
    std::map<SensorReading::SensorType, SensorStatistics> statsMap;
    
    // Group by type
    std::map<SensorReading::SensorType, std::vector<SensorReading>> grouped;
    for (const auto& reading : readings) {
        grouped[reading.getType()].push_back(reading);
    }
    
    // Calculate stats for each group
    for (const auto& pair : grouped) {
        statsMap[pair.first] = calculateStatistics(pair.second);
    }
    
    return statsMap;
}

std::map<std::string, SensorStatistics> 
SensorDataProcessor::calculateStatisticsBySensorId(
    const std::vector<SensorReading>& readings) const {
    
    std::map<std::string, SensorStatistics> statsMap;
    
    // Group by sensor ID
    std::map<std::string, std::vector<SensorReading>> grouped;
    for (const auto& reading : readings) {
        grouped[reading.getSensorId()].push_back(reading);
    }
    
    // Calculate stats for each group
    for (const auto& pair : grouped) {
        statsMap[pair.first] = calculateStatistics(pair.second);
    }
    
    return statsMap;
}

std::vector<SensorReading> SensorDataProcessor::removeOutliers(
    const std::vector<SensorReading>& readings) const {
    
    if (readings.size() < 4) {
        return readings;  // Need at least 4 points for IQR
    }
    
    std::vector<double> values;
    values.reserve(readings.size());
    for (const auto& reading : readings) {
        values.push_back(reading.getValue());
    }
    
    std::sort(values.begin(), values.end());
    
    double q1, q3;
    calculateQuartiles(values, q1, q3);
    
    double iqr = q3 - q1;
    double lowerBound = q1 - 1.5 * iqr;
    double upperBound = q3 + 1.5 * iqr;
    
    std::vector<SensorReading> filtered;
    std::copy_if(readings.begin(), readings.end(),
                 std::back_inserter(filtered),
                 [lowerBound, upperBound](const SensorReading& r) {
                     double val = r.getValue();
                     return val >= lowerBound && val <= upperBound;
                 });
    
    return filtered;
}

void SensorDataProcessor::normalizeValues(
    std::vector<SensorReading>& readings) const {
    
    if (readings.empty()) {
        return;
    }
    
    // Find min and max
    auto minmax = std::minmax_element(
        readings.begin(), readings.end(),
        [](const SensorReading& a, const SensorReading& b) {
            return a.getValue() < b.getValue();
        });
    
    double minVal = minmax.first->getValue();
    double maxVal = minmax.second->getValue();
    double range = maxVal - minVal;
    
    if (range == 0.0) {
        return;  // All values are the same
    }
    
    // Normalize to 0-1 range
    for (auto& reading : readings) {
        double normalized = (reading.getValue() - minVal) / range;
        reading.setValue(normalized);
    }
}

double SensorDataProcessor::calculateMedian(std::vector<double>& values) const {
    if (values.empty()) {
        return 0.0;
    }
    
    size_t n = values.size();
    if (n % 2 == 0) {
        return (values[n/2 - 1] + values[n/2]) / 2.0;
    } else {
        return values[n/2];
    }
}

void SensorDataProcessor::calculateQuartiles(
    const std::vector<double>& sortedValues,
    double& q1, double& q3) const {
    
    size_t n = sortedValues.size();
    
    // Q1: median of first half
    size_t q1Index = n / 4;
    if (n % 4 == 0 || n % 4 == 1) {
        q1 = sortedValues[q1Index];
    } else {
        q1 = (sortedValues[q1Index] + sortedValues[q1Index + 1]) / 2.0;
    }
    
    // Q3: median of second half
    size_t q3Index = (3 * n) / 4;
    if (n % 4 == 0 || n % 4 == 3) {
        q3 = sortedValues[q3Index];
    } else {
        q3 = (sortedValues[q3Index] + sortedValues[q3Index + 1]) / 2.0;
    }
}

