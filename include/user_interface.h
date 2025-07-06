#pragma once

#include <LiquidCrystal.h>
#include "liquid_crystal_ext.h" // Extended LiquidCrystal library for Polish characters
#include <math.h> // For NAN
#include "persistence_manager.h"

class UserInterface {
public:
    enum State { MAIN_SCREEN, SETTINGS_MENU, EDIT_SETTING };

    UserInterface(PolishLCD* lcd, PersistenceManager* persistence_manager);
    void updateDisplay();
    void handleSelect();
    void handleUp();
    void handleDown();
    void handleNext();
    void handlePrev();

    void addSetting(const char* name, int* value_ptr, int min_val, int max_val);

    void setFanState(bool state) { is_fan_on_ = state; }
    void setExternalTemperature(float temp) { external_temp_ = temp; }
    void setInternalTemperature(float temp) { internal_temp_ = temp; }

private:
    // UI settings
    static const int MAX_SETTINGS = 8;
    struct Setting {
        const char* name;
        int* value_ptr;
        int min_val;
        int max_val;
        int step = 1; // Default step for adjustment
    };

    // LCD and persistence manager pointers
    PolishLCD* lcd_;
    PersistenceManager* persistence_manager_;
    // Temperature readings
    float external_temp_ = NAN; // Use NAN to indicate no reading
    float internal_temp_ = NAN; // Use NAN to indicate no reading

    // Fan state
    bool is_fan_on_ = false;

    // Current state and settings
    State current_state_;
    int current_index_;
    Setting settings_[MAX_SETTINGS];
    int num_settings_ = 0;

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
};
