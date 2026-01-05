#ifndef SENSOR_READING_H
#define SENSOR_READING_H

#include <string>
#include <chrono>
#include <cstdint>

/**
 * @brief Represents a single sensor reading with timestamp and value
 * 
 * This class models sensor data commonly found in defense/undersea systems,
 * including timestamp, sensor ID, and measurement value.
 */
class SensorReading {
public:
    // Sensor types for different measurement categories
    enum class SensorType {
        TEMPERATURE,
        PRESSURE,
        DEPTH,
        SONAR,
        ACCELEROMETER,
        GYROSCOPE
    };

    /**
     * @brief Default constructor
     */
    SensorReading();

    /**
     * @brief Parameterized constructor
     * @param sensorId Unique identifier for the sensor
     * @param type Type of sensor measurement
     * @param value Measurement value
     * @param timestamp Unix timestamp in milliseconds
     */
    SensorReading(const std::string& sensorId, SensorType type, 
                  double value, int64_t timestamp);

    // Getters
    const std::string& getSensorId() const { return sensorId_; }
    SensorType getType() const { return type_; }
    double getValue() const { return value_; }
    int64_t getTimestamp() const { return timestamp_; }

    // Setters
    void setSensorId(const std::string& sensorId) { sensorId_ = sensorId; }
    void setType(SensorType type) { type_ = type; }
    void setValue(double value) { value_ = value; }
    void setTimestamp(int64_t timestamp) { timestamp_ = timestamp; }

    /**
     * @brief Convert sensor type to string representation
     */
    static std::string typeToString(SensorType type);

    /**
     * @brief Parse sensor type from string
     */
    static SensorType stringToType(const std::string& str);

    /**
     * @brief Check if reading is valid (non-null sensor ID, reasonable timestamp)
     */
    bool isValid() const;

private:
    std::string sensorId_;
    SensorType type_;
    double value_;
    int64_t timestamp_;  // Unix timestamp in milliseconds
};

#endif // SENSOR_READING_H

