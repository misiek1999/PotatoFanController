#pragma once

#include "persistence_manager.h"


// Variables to hold settings
inline PersistenceManager* getSingletonPersistenceManager() {
    static PersistenceManager instance; // Create a singleton instance of PersistenceManager
    return &instance;
}


// c functions of PersistenceManager singleton
inline float getMinimalExternalTemperature() {
    return getSingletonPersistenceManager()->getMinimalExternalTemperature();
}

inline float getMaximalExternalTemperature() {
    return getSingletonPersistenceManager()->getMaximalExternalTemperature();
}

inline float getTemperatureDifferenceHysteresis() {
    return getSingletonPersistenceManager()->getTemperatureDifferenceHysteresis();
}

inline size_t getSwitchTimeHysteresis() {
    return getSingletonPersistenceManager()->getSwitchTimeHysteresis();
}

inline void setMinimalExternalTemperature(float value) {
    getSingletonPersistenceManager()->setMinimalExternalTemperature(value);
}

inline void setMaximalExternalTemperature(float value) {
    getSingletonPersistenceManager()->setMaximalExternalTemperature(value);
}

inline void setTemperatureDifferenceHysteresis(float value) {
    getSingletonPersistenceManager()->setTemperatureDifferenceHysteresis(value);
}

inline void setSwitchTimeHysteresis(size_t value) {
    getSingletonPersistenceManager()->setSwitchTimeHysteresis(value);
}
