#include "test_SensorDataProcessor.h"
#include "SensorDataProcessor.h"
#include "SensorReading.h"
#include <iostream>
#include <cmath>

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "FAIL: " << message << "\n"; \
            return false; \
        } \
    } while(0)

#define ASSERT_APPROX(a, b, epsilon, message) \
    ASSERT(std::abs((a) - (b)) < epsilon, message)

bool testFilterByType() {
    SensorDataProcessor processor;
    
    std::vector<SensorReading> readings = {
        SensorReading("S1", SensorReading::SensorType::TEMPERATURE, 20.0, 1000),
        SensorReading("S2", SensorReading::SensorType::PRESSURE, 1013.0, 2000),
        SensorReading("S3", SensorReading::SensorType::TEMPERATURE, 25.0, 3000),
        SensorReading("S4", SensorReading::SensorType::DEPTH, 100.0, 4000)
    };
    
    auto filtered = processor.filterByType(readings, SensorReading::SensorType::TEMPERATURE);
    ASSERT(filtered.size() == 2, "Should filter to 2 temperature readings");
    ASSERT(filtered[0].getType() == SensorReading::SensorType::TEMPERATURE, 
           "All filtered readings should be TEMPERATURE type");
    
    return true;
}

bool testFilterBySensorId() {
    SensorDataProcessor processor;
    
    std::vector<SensorReading> readings = {
        SensorReading("S1", SensorReading::SensorType::TEMPERATURE, 20.0, 1000),
        SensorReading("S2", SensorReading::SensorType::PRESSURE, 1013.0, 2000),
        SensorReading("S1", SensorReading::SensorType::TEMPERATURE, 25.0, 3000),
        SensorReading("S3", SensorReading::SensorType::DEPTH, 100.0, 4000)
    };
    
    auto filtered = processor.filterBySensorId(readings, "S1");
    ASSERT(filtered.size() == 2, "Should filter to 2 readings from S1");
    ASSERT(filtered[0].getSensorId() == "S1", "All filtered readings should be from S1");
    
    return true;
}

bool testFilterByValueRange() {
    SensorDataProcessor processor;
    
    std::vector<SensorReading> readings = {
        SensorReading("S1", SensorReading::SensorType::TEMPERATURE, 20.0, 1000),
        SensorReading("S2", SensorReading::SensorType::TEMPERATURE, 25.0, 2000),
        SensorReading("S3", SensorReading::SensorType::TEMPERATURE, 30.0, 3000),
        SensorReading("S4", SensorReading::SensorType::TEMPERATURE, 35.0, 4000)
    };
    
    auto filtered = processor.filterByValueRange(readings, 22.0, 32.0);
    ASSERT(filtered.size() == 2, "Should filter to 2 readings in range");
    ASSERT(filtered[0].getValue() == 25.0, "First filtered value should be 25.0");
    ASSERT(filtered[1].getValue() == 30.0, "Second filtered value should be 30.0");
    
    return true;
}

bool testCalculateStatistics() {
    SensorDataProcessor processor;
    
    std::vector<SensorReading> readings = {
        SensorReading("S1", SensorReading::SensorType::TEMPERATURE, 10.0, 1000),
        SensorReading("S2", SensorReading::SensorType::TEMPERATURE, 20.0, 2000),
        SensorReading("S3", SensorReading::SensorType::TEMPERATURE, 30.0, 3000),
        SensorReading("S4", SensorReading::SensorType::TEMPERATURE, 40.0, 4000)
    };
    
    auto stats = processor.calculateStatistics(readings);
    ASSERT(stats.count == 4, "Count should be 4");
    ASSERT(stats.min == 10.0, "Min should be 10.0");
    ASSERT(stats.max == 40.0, "Max should be 40.0");
    ASSERT_APPROX(stats.mean, 25.0, 0.01, "Mean should be 25.0");
    ASSERT_APPROX(stats.median, 25.0, 0.01, "Median should be 25.0");
    
    return true;
}

bool testCalculateStatisticsByType() {
    SensorDataProcessor processor;
    
    std::vector<SensorReading> readings = {
        SensorReading("S1", SensorReading::SensorType::TEMPERATURE, 20.0, 1000),
        SensorReading("S2", SensorReading::SensorType::TEMPERATURE, 25.0, 2000),
        SensorReading("S3", SensorReading::SensorType::PRESSURE, 1013.0, 3000),
        SensorReading("S4", SensorReading::SensorType::PRESSURE, 1015.0, 4000)
    };
    
    auto statsByType = processor.calculateStatisticsByType(readings);
    ASSERT(statsByType.size() == 2, "Should have stats for 2 types");
    ASSERT(statsByType[SensorReading::SensorType::TEMPERATURE].count == 2,
           "Temperature should have 2 readings");
    ASSERT(statsByType[SensorReading::SensorType::PRESSURE].count == 2,
           "Pressure should have 2 readings");
    
    return true;
}

bool testRemoveOutliers() {
    SensorDataProcessor processor;
    
    // Create data with obvious outliers
    std::vector<SensorReading> readings;
    for (int i = 0; i < 10; ++i) {
        readings.emplace_back("S1", SensorReading::SensorType::TEMPERATURE, 
                             20.0 + i, 1000 + i * 100);
    }
    // Add outliers
    readings.emplace_back("S1", SensorReading::SensorType::TEMPERATURE, 1000.0, 2000);
    readings.emplace_back("S1", SensorReading::SensorType::TEMPERATURE, -1000.0, 3000);
    
    auto filtered = processor.removeOutliers(readings);
    ASSERT(filtered.size() <= readings.size(), "Filtered should have same or fewer readings");
    // Outliers should be removed
    bool hasOutlier = false;
    for (const auto& r : filtered) {
        if (r.getValue() > 100 || r.getValue() < 0) {
            hasOutlier = true;
            break;
        }
    }
    ASSERT(!hasOutlier, "Outliers should be removed");
    
    return true;
}

bool testProcess() {
    SensorDataProcessor processor;
    
    std::vector<SensorReading> readings = {
        SensorReading("S1", SensorReading::SensorType::TEMPERATURE, 20.0, 1000),
        SensorReading("", SensorReading::SensorType::TEMPERATURE, 25.0, 2000),  // Invalid
        SensorReading("S3", SensorReading::SensorType::TEMPERATURE, 30.0, 0),   // Invalid
        SensorReading("S4", SensorReading::SensorType::TEMPERATURE, 35.0, 4000)
    };
    
    auto processed = processor.process(readings);
    ASSERT(processed.size() <= readings.size(), "Processed should have same or fewer readings");
    // All processed readings should be valid
    for (const auto& r : processed) {
        ASSERT(r.isValid(), "All processed readings should be valid");
    }
    
    return true;
}

std::pair<int, int> runSensorDataProcessorTests() {
    int testsRun = 0;
    int testsPassed = 0;
    
    auto runTest = [&](const std::string& name, bool (*test)()) {
        testsRun++;
        std::cout << "  " << name << "... ";
        if (test()) {
            std::cout << "PASS\n";
            testsPassed++;
        } else {
            std::cout << "FAIL\n";
        }
    };
    
    runTest("Filter By Type", testFilterByType);
    runTest("Filter By Sensor ID", testFilterBySensorId);
    runTest("Filter By Value Range", testFilterByValueRange);
    runTest("Calculate Statistics", testCalculateStatistics);
    runTest("Calculate Statistics By Type", testCalculateStatisticsByType);
    runTest("Remove Outliers", testRemoveOutliers);
    runTest("Process", testProcess);
    
    return {testsRun, testsPassed};
}

