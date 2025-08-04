#include "persistence_manager.h"
#include "log.h"
#include <string.h>

PersistenceManager::PersistenceManager() {
    // clear local data structure
    memset(&data_, 0, sizeof(data_));

    checkIsDataPresent();       // Check if data is present in EEPROM
    if (!data_.has_data) {
        loadDefaults();          // Load default values if no data is present
    } else {
        loadData();             // Load existing data from EEPROM
    }
    LOG_INFO("PersistenceManager initialized");
}

void PersistenceManager::checkIsDataPresent() {
    EEPROM.get(0, data_); // Read the first byte to check if data is present
    if (data_.has_data) {
        LOG_INFO("Data is present in EEPROM");
    } else {
        LOG_INFO("No data found in EEPROM, loading defaults");
    }
    if (!checkCRC()) { // Check if the data integrity is valid
        LOG_WARNING("CRC check failed, loading defaults");
        data_.has_data = false; // Reset the flag if CRC check fails
    } else {
        LOG_INFO("CRC check passed");
    }
}

void PersistenceManager::loadDefaults() {
    data_.has_data = true; // Set the flag to indicate that data is present
    data_.minimal_external_temperature = DEFAULT_MINIMAL_EXTERNAL_TEMPERATURE;
    data_.temperature_difference_hysteresis = DEFAULT_TEMPERATURE_DIFFERENCE_HYSTERESIS;
    data_.switch_time_hysteresis = DEFAULT_SWITCH_TIME_HYSTERESIS;
    LOG_INFO("Default values loaded ");
}

void PersistenceManager::saveData() {
    EEPROM.put(0, data_); // Save data to EEPROM
    updateCRC(); // Update the CRC after saving data
    LOG_INFO("Data saved to EEPROM");
}

void PersistenceManager::loadData() {
    const auto is_data_corrupted = !checkCRC(); // Check if the data is corrupted
    if (is_data_corrupted) {
        LOG_ERROR("Data is corrupted, loading defaults");
        loadDefaults(); // Load default values if data is corrupted
        saveData(); // Save the default values to EEPROM
        return; // Exit the function after loading defaults
    }
    EEPROM.get(0, data_); // Load data from EEPROM
    LOG_INFO("Data loaded from EEPROM");
}

float PersistenceManager::getMinimalExternalTemperature() const {
    return data_.minimal_external_temperature;
}

float PersistenceManager::getTemperatureDifferenceHysteresis() const {
    return data_.temperature_difference_hysteresis;
}

size_t PersistenceManager::getSwitchTimeHysteresis() const {
    return data_.switch_time_hysteresis;
}

void PersistenceManager::setMinimalExternalTemperature(float value) {
    data_.minimal_external_temperature = value;
    saveData(); // Save updated data to EEPROM
    LOG_INFO("Minimal external temperature set to %F", value);
}

float PersistenceManager::getMaximalExternalTemperature() const {
    return data_.maximal_external_temperature;
}

void PersistenceManager::setMaximalExternalTemperature(float value) {
    data_.maximal_external_temperature = value;
    saveData(); // Save updated data to EEPROM
    LOG_INFO("Maximal external temperature set to %F", value);
}

void PersistenceManager::setTemperatureDifferenceHysteresis(float value) {
    data_.temperature_difference_hysteresis = value;
    saveData(); // Save updated data to EEPROM
    LOG_INFO("Temperature difference hysteresis set to %F", value);
}

void PersistenceManager::setSwitchTimeHysteresis(size_t value) {
    data_.switch_time_hysteresis = value;
    saveData(); // Save updated data to EEPROM
    LOG_INFO("Switch time hysteresis set to %l seconds", static_cast<long>(value));
}

bool PersistenceManager::checkCRC() const {
    uint32_t stored_crc = 0;
    EEPROM.get(sizeof(PersistenceData), stored_crc); // Read the stored CRC from EEPROM
    CRC32 crc;
    crc.add(reinterpret_cast<const uint8_t*>(&data_), sizeof(data_));
    const auto calculated_crc = crc.calc(); // Calculate the CRC
    LOG_INFO("Stored CRC: %u, Calculated CRC: %u", (unsigned long)stored_crc,
                                                   (unsigned long)calculated_crc);
    return stored_crc == calculated_crc; // Compare stored and calculated CRC
}

void PersistenceManager::updateCRC() {
    CRC32 crc;
    crc.add(reinterpret_cast<const uint8_t*>(&data_), sizeof(data_));
    const auto calculated_crc = crc.calc(); // Calculate the CRC
    EEPROM.put(sizeof(PersistenceData), calculated_crc); // Save the calculated CRC to EEPROM
    LOG_INFO("CRC updated and saved to EEPROM: %u", (unsigned long)calculated_crc);
}
