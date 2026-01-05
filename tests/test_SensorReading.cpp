#include "test_SensorReading.h"
#include "SensorReading.h"
#include <iostream>
#include <cassert>

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "FAIL: " << message << "\n"; \
            return false; \
        } \
    } while(0)

bool testDefaultConstructor() {
    SensorReading reading;
    ASSERT(reading.getSensorId() == "", "Default sensor ID should be empty");
    ASSERT(reading.getValue() == 0.0, "Default value should be 0.0");
    ASSERT(reading.getTimestamp() == 0, "Default timestamp should be 0");
    ASSERT(!reading.isValid(), "Default reading should be invalid");
    return true;
}

bool testParameterizedConstructor() {
    SensorReading reading("SENSOR_001", SensorReading::SensorType::TEMPERATURE, 25.5, 1234567890);
    ASSERT(reading.getSensorId() == "SENSOR_001", "Sensor ID should match");
    ASSERT(reading.getType() == SensorReading::SensorType::TEMPERATURE, "Type should match");
    ASSERT(reading.getValue() == 25.5, "Value should match");
    ASSERT(reading.getTimestamp() == 1234567890, "Timestamp should match");
    ASSERT(reading.isValid(), "Reading should be valid");
    return true;
}

bool testTypeToString() {
    ASSERT(SensorReading::typeToString(SensorReading::SensorType::TEMPERATURE) == "TEMPERATURE",
           "TEMPERATURE type should convert correctly");
    ASSERT(SensorReading::typeToString(SensorReading::SensorType::PRESSURE) == "PRESSURE",
           "PRESSURE type should convert correctly");
    ASSERT(SensorReading::typeToString(SensorReading::SensorType::DEPTH) == "DEPTH",
           "DEPTH type should convert correctly");
    return true;
}

bool testStringToType() {
    ASSERT(SensorReading::stringToType("TEMPERATURE") == SensorReading::SensorType::TEMPERATURE,
           "TEMPERATURE string should convert correctly");
    ASSERT(SensorReading::stringToType("PRESSURE") == SensorReading::SensorType::PRESSURE,
           "PRESSURE string should convert correctly");
    ASSERT(SensorReading::stringToType("DEPTH") == SensorReading::SensorType::DEPTH,
           "DEPTH string should convert correctly");
    return true;
}

bool testIsValid() {
    SensorReading valid("SENSOR_001", SensorReading::SensorType::TEMPERATURE, 25.5, 1234567890);
    ASSERT(valid.isValid(), "Reading with valid data should be valid");
    
    SensorReading invalid1("", SensorReading::SensorType::TEMPERATURE, 25.5, 1234567890);
    ASSERT(!invalid1.isValid(), "Reading with empty sensor ID should be invalid");
    
    SensorReading invalid2("SENSOR_001", SensorReading::SensorType::TEMPERATURE, 25.5, 0);
    ASSERT(!invalid2.isValid(), "Reading with zero timestamp should be invalid");
    
    return true;
}

bool testSetters() {
    SensorReading reading;
    reading.setSensorId("SENSOR_002");
    reading.setType(SensorReading::SensorType::PRESSURE);
    reading.setValue(1013.25);
    reading.setTimestamp(9876543210);
    
    ASSERT(reading.getSensorId() == "SENSOR_002", "Setter should update sensor ID");
    ASSERT(reading.getType() == SensorReading::SensorType::PRESSURE, "Setter should update type");
    ASSERT(reading.getValue() == 1013.25, "Setter should update value");
    ASSERT(reading.getTimestamp() == 9876543210, "Setter should update timestamp");
    return true;
}

std::pair<int, int> runSensorReadingTests() {
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
    
    runTest("Default Constructor", testDefaultConstructor);
    runTest("Parameterized Constructor", testParameterizedConstructor);
    runTest("Type To String", testTypeToString);
    runTest("String To Type", testStringToType);
    runTest("Is Valid", testIsValid);
    runTest("Setters", testSetters);
    
    return {testsRun, testsPassed};
}

