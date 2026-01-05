#include "SensorReading.h"
#include <stdexcept>
#include <map>

SensorReading::SensorReading()
    : sensorId_(""), type_(SensorType::TEMPERATURE), value_(0.0), timestamp_(0) {
}

SensorReading::SensorReading(const std::string& sensorId, SensorType type,
                             double value, int64_t timestamp)
    : sensorId_(sensorId), type_(type), value_(value), timestamp_(timestamp) {
}

std::string SensorReading::typeToString(SensorType type) {
    static const std::map<SensorType, std::string> typeMap = {
        {SensorType::TEMPERATURE, "TEMPERATURE"},
        {SensorType::PRESSURE, "PRESSURE"},
        {SensorType::DEPTH, "DEPTH"},
        {SensorType::SONAR, "SONAR"},
        {SensorType::ACCELEROMETER, "ACCELEROMETER"},
        {SensorType::GYROSCOPE, "GYROSCOPE"}
    };
    
    auto it = typeMap.find(type);
    if (it != typeMap.end()) {
        return it->second;
    }
    return "UNKNOWN";
}

SensorReading::SensorType SensorReading::stringToType(const std::string& str) {
    static const std::map<std::string, SensorType> stringMap = {
        {"TEMPERATURE", SensorType::TEMPERATURE},
        {"PRESSURE", SensorType::PRESSURE},
        {"DEPTH", SensorType::DEPTH},
        {"SONAR", SensorType::SONAR},
        {"ACCELEROMETER", SensorType::ACCELEROMETER},
        {"GYROSCOPE", SensorType::GYROSCOPE}
    };
    
    auto it = stringMap.find(str);
    if (it != stringMap.end()) {
        return it->second;
    }
    return SensorType::TEMPERATURE;  // Default fallback
}

bool SensorReading::isValid() const {
    return !sensorId_.empty() && timestamp_ > 0;
}

