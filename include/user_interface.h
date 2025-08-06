#pragma once

#include <LiquidCrystal.h>
#include "liquid_crystal_ext.h" // Extended LiquidCrystal library for Polish characters
#include <math.h> // For NAN
#include "persistence_manager.h"
#include "setting.h"

class UserInterface {
public:
    enum State { MAIN_SCREEN, SETTINGS_MENU, EDIT_SETTING };

    explicit UserInterface(PolishLCD* lcd);
    void updateDisplay();
    void handleSelect();
    void handleUp();
    void handleDown();
    void handleNext();
    void handlePrev();

    void setFanState(bool state) { is_fan_on_ = state; }
    void setExternalTemperature(float temp) { external_temp_ = temp; }
    void setInternalTemperature(float temp) { internal_temp_ = temp; }

private:

    // LCD and persistence manager pointers
    PolishLCD* lcd_;
    // Temperature readings
    float external_temp_ = NAN; // Use NAN to indicate no reading
    float internal_temp_ = NAN; // Use NAN to indicate no reading

    // Fan state
    bool is_fan_on_ = false;

    // Current state and settings
    State current_state_;
    ISetting** settings_array_; // Array of settings
    size_t current_setting_ = 0; // Index of the current setting being edited
    size_t total_num_settings_ = 0; // Total number of settings

    // Helper methods
    void showMainScreen();
    void showSettingsMenu();
    void showEditSetting();
    void enterSettingsMenu();
    void exitSettingsMenu();
    void enterEditSetting();
    void exitEditSetting();
    void navigateSettings(int direction);
    void adjustSetting(int delta);
    void saveSetting();
    void discardSetting();
};
