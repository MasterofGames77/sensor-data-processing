#include <iostream>
#include <cassert>
#include "test_SensorReading.h"
#include "test_SensorDataProcessor.h"

/**
 * Simple test framework for unit tests
 */
int main() {
    std::cout << "Running unit tests...\n\n";
    
    int testsRun = 0;
    int testsPassed = 0;
    
    // Run SensorReading tests
    std::cout << "=== SensorReading Tests ===\n";
    auto readingResults = runSensorReadingTests();
    testsRun += readingResults.first;
    testsPassed += readingResults.second;
    
    // Run SensorDataProcessor tests
    std::cout << "\n=== SensorDataProcessor Tests ===\n";
    auto processorResults = runSensorDataProcessorTests();
    testsRun += processorResults.first;
    testsPassed += processorResults.second;
    
    // Summary
    std::cout << "\n=== Test Summary ===\n";
    std::cout << "Tests run: " << testsRun << "\n";
    std::cout << "Tests passed: " << testsPassed << "\n";
    std::cout << "Tests failed: " << (testsRun - testsPassed) << "\n";
    
    if (testsPassed == testsRun) {
        std::cout << "\n✓ All tests passed!\n";
        return 0;
    } else {
        std::cout << "\n✗ Some tests failed!\n";
        return 1;
    }
}

