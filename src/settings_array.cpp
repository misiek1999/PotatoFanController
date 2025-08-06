#include "settings_array.h"
#include "setting.h"
#include "log.h"
#include "persistence_manager_instance.h" // For accessing persistence manager functions

// Constants for settings
const float TEMPERATURE_STEP = 0.5f; // Minimum external temperature setting [C]
const float TIME_DIFFERENCE_HYSTERESIS_STEP = 1U; // Time difference hysteresis setting [min]
const float TEMPERATURE_DIFFERENCE_HYSTERESIS_STEP = 0.5f; // Temperature difference hysteresis setting [C]


ISetting** createSettings(size_t& count)
{
    static Setting<float, 32> minimal_external_temperature_setting(
        "Minimal External Temp",
        "Min temp zewn",
        getMinimalExternalTemperature,
        setMinimalExternalTemperature,
        TEMPERATURE_STEP
    );
    static Setting<float, 32> maximal_external_temperature_setting(
        "Maximal External Temp",
        "Max temp zewn",
        getMaximalExternalTemperature,
        setMaximalExternalTemperature,
        TEMPERATURE_STEP
    );
    static Setting<float, 32> temperature_difference_hysteresis_setting(
        "Temperature Difference Hyst",
        "Temp różnica",
        getTemperatureDifferenceHysteresis,
        setTemperatureDifferenceHysteresis,
        TEMPERATURE_DIFFERENCE_HYSTERESIS_STEP
    );

    static Setting<size_t, 32> switch_time_hysteresis_setting(
        "Switch Time Hysteresis",
        "Czas różnica",
        getSwitchTimeHysteresis,
        setSwitchTimeHysteresis,
        TIME_DIFFERENCE_HYSTERESIS_STEP
    );

    static Setting<void> reset_settings(
        "Reset Settings",
        "Resetuj ustw",
        resetSettings
    );

    static ISetting* settings[] = {
        &minimal_external_temperature_setting,
        &maximal_external_temperature_setting,
        &temperature_difference_hysteresis_setting,
        &switch_time_hysteresis_setting,
        &reset_settings
    };
    count = sizeof(settings) / sizeof(settings[0]); // Set the count of settings
    LOG_INFO("Created %zu settings", count);
    return settings;
}
