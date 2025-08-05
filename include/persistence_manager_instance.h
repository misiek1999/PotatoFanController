#pragma once

#include "persistence_manager.h"


// Variables to hold settings
inline PersistenceManager* getPersistenceManagerInstance() {
    static PersistenceManager instance; // Create a singleton instance of PersistenceManager
    return &instance;
}


// c functions of PersistenceManager singleton
inline float getMinimalExternalTemperature() {
    return getPersistenceManagerInstance()->getMinimalExternalTemperature();
}

inline float getMaximalExternalTemperature() {
    return getPersistenceManagerInstance()->getMaximalExternalTemperature();
}

inline float getTemperatureDifferenceHysteresis() {
    return getPersistenceManagerInstance()->getTemperatureDifferenceHysteresis();
}

inline size_t getSwitchTimeHysteresis() {
    return getPersistenceManagerInstance()->getSwitchTimeHysteresis();
}

inline void setMinimalExternalTemperature(float value) {
    getPersistenceManagerInstance()->setMinimalExternalTemperature(value);
}

inline void setMaximalExternalTemperature(float value) {
    getPersistenceManagerInstance()->setMaximalExternalTemperature(value);
}

inline void setTemperatureDifferenceHysteresis(float value) {
    getPersistenceManagerInstance()->setTemperatureDifferenceHysteresis(value);
}

inline void setSwitchTimeHysteresis(size_t value) {
    getPersistenceManagerInstance()->setSwitchTimeHysteresis(value);
}
