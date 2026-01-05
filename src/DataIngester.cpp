#include "DataIngester.h"
#include <sstream>
#include <random>
#include <chrono>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <cctype>

DataIngester::DataIngester() {
}

std::vector<SensorReading> DataIngester::readFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath);
    }

    std::vector<SensorReading> readings;
    std::string line;
    
    // Skip header line if present
    if (std::getline(file, line)) {
        // Check if it's a header (contains "sensor_id" or "timestamp")
        std::transform(line.begin(), line.end(), line.begin(),
                      [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (line.find("sensor_id") == std::string::npos) {
            // Not a header, parse as data
            file.seekg(0);
        }
    }

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;  // Skip empty lines and comments
        }
        
        try {
            SensorReading reading = parseCSVLine(line);
            if (reading.isValid()) {
                readings.push_back(reading);
            }
        } catch (const std::exception&) {
            // Skip malformed lines, continue processing
            continue;
        }
    }

    file.close();
    return readings;
}

SensorReading DataIngester::parseCSVLine(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(iss, token, ',')) {
        // Trim whitespace
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        tokens.push_back(token);
    }

    if (tokens.size() < 4) {
        throw std::runtime_error("Invalid CSV format: expected at least 4 columns");
    }

    std::string sensorId = tokens[0];
    SensorReading::SensorType type = SensorReading::stringToType(tokens[1]);
    double value = std::stod(tokens[2]);
    int64_t timestamp = std::stoll(tokens[3]);

    return SensorReading(sensorId, type, value, timestamp);
}

std::vector<SensorReading> DataIngester::generateSimulatedData(
    size_t count,
    const std::vector<std::string>& sensorIds,
    const std::vector<SensorReading::SensorType>& types) {
    
    if (sensorIds.empty() || types.empty()) {
        throw std::runtime_error("Sensor IDs and types must not be empty");
    }

    std::vector<SensorReading> readings;
    readings.reserve(count);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> sensorDist(0, sensorIds.size() - 1);
    std::uniform_int_distribution<size_t> typeDist(0, types.size() - 1);

    int64_t baseTimestamp = getCurrentTimestamp();
    std::uniform_int_distribution<int64_t> timeDist(0, 3600000);  // 1 hour range

    // Value ranges for different sensor types
    std::map<SensorReading::SensorType, std::pair<double, double>> valueRanges = {
        {SensorReading::SensorType::TEMPERATURE, {0.0, 100.0}},
        {SensorReading::SensorType::PRESSURE, {0.0, 1000.0}},
        {SensorReading::SensorType::DEPTH, {0.0, 5000.0}},
        {SensorReading::SensorType::SONAR, {0.0, 10000.0}},
        {SensorReading::SensorType::ACCELEROMETER, {-10.0, 10.0}},
        {SensorReading::SensorType::GYROSCOPE, {-180.0, 180.0}}
    };

    for (size_t i = 0; i < count; ++i) {
        std::string sensorId = sensorIds[sensorDist(gen)];
        SensorReading::SensorType type = types[typeDist(gen)];
        
        auto range = valueRanges[type];
        double value = generateRandomValue(range.first, range.second);
        
        int64_t timestamp = baseTimestamp + timeDist(gen);

        readings.emplace_back(sensorId, type, value, timestamp);
    }

    return readings;
}

bool DataIngester::writeToFile(const std::vector<SensorReading>& readings,
                                const std::string& filepath) const {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    // Write header
    file << "sensor_id,type,value,timestamp\n";

    // Write data
    for (const auto& reading : readings) {
        file << reading.getSensorId() << ","
             << SensorReading::typeToString(reading.getType()) << ","
             << reading.getValue() << ","
             << reading.getTimestamp() << "\n";
    }

    file.close();
    return true;
}

int64_t DataIngester::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

double DataIngester::generateRandomValue(double min, double max) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(gen);
}

