#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include <CRC32.h>

constexpr size_t EEPROM_SIZE = 512; // Define the size of EEPROM

constexpr float DEFAULT_MINIMAL_EXTERNAL_TEMPERATURE = 4.0f; // Default minimal external temperature
constexpr float DEFAULT_MAXIMAL_EXTERNAL_TEMPERATURE = 20.0f; // Default maximal external temperature
constexpr float DEFAULT_TEMPERATURE_DIFFERENCE_HYSTERESIS = 1.0f; // Default temperature difference hysteresis
constexpr size_t DEFAULT_SWITCH_TIME_HYSTERESIS = 5 * 60; // Default switch time hysteresis in seconds
struct PersistenceData {
    uint8_t has_data = 0; // Flag to indicate if data is present
    float minimal_external_temperature = DEFAULT_MINIMAL_EXTERNAL_TEMPERATURE;
    float maximal_external_temperature = DEFAULT_MAXIMAL_EXTERNAL_TEMPERATURE;
    float temperature_difference_hysteresis = DEFAULT_TEMPERATURE_DIFFERENCE_HYSTERESIS;
    size_t switch_time_hysteresis = DEFAULT_SWITCH_TIME_HYSTERESIS;
};

class PersistenceManager {
public:
    PersistenceManager();
    ~PersistenceManager() = default;

    // Load data from EEPROM
    float getMinimalExternalTemperature() const;
    float getTemperatureDifferenceHysteresis() const;
    size_t getSwitchTimeHysteresis() const;
    float getMaximalExternalTemperature() const;

    // Set data and save to EEPROM
    void setMinimalExternalTemperature(float value);
    void setMaximalExternalTemperature(float value);
    void setTemperatureDifferenceHysteresis(float value);
    void setSwitchTimeHysteresis(size_t value);

private:
    void checkIsDataPresent();
    void loadDefaults();
    void saveData();
    void loadData();

    bool checkCRC() const;
    void updateCRC();

    PersistenceData data_; // Data structure to hold persistence data

};
