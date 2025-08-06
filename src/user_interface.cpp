#include "user_interface.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <liquid_crystal_ext.h>
#include "log.h"
#include "settings_array.h"

UserInterface::UserInterface(PolishLCD* lcd)
    : lcd_(lcd), current_state_(MAIN_SCREEN), current_setting_(0) {
    if (!lcd_) {
        LOG_FATAL("LCD pointer is null");
    }
    // Get array of settings
    settings_array_ = createSettings(total_num_settings_);
    if (total_num_settings_ == 0) {
        LOG_WARNING("No settings available");
    } else {
        LOG_INFO("Loaded %d settings", total_num_settings_);
    }

    LOG_INFO("UserInterface initialized");
}

void UserInterface::updateDisplay() {
    switch (current_state_) {
        case MAIN_SCREEN:
            showMainScreen();
            break;
        case SETTINGS_MENU:
            showSettingsMenu();
            break;
        case EDIT_SETTING:
            showEditSetting();
            break;
    }
}

void UserInterface::handleSelect() {
    switch (current_state_) {
        case MAIN_SCREEN:
            enterSettingsMenu();
            break;
        case SETTINGS_MENU:
            if (current_setting_ < total_num_settings_) {
                enterEditSetting();
            } else {
                exitSettingsMenu();
            }
            break;
        case EDIT_SETTING:
            saveSetting();
            exitEditSetting();
            break;
    }
    updateDisplay();
}

void UserInterface::handleUp() {
    switch (current_state_) {
        case SETTINGS_MENU:
            navigateSettings(-1);
            break;
        case EDIT_SETTING:
            adjustSetting(1);
            break;
        default:
            LOG_DEBUG("No action for UP in current state");
            break;
    }
    updateDisplay();
}

void UserInterface::handleDown() {
    switch (current_state_) {
        case SETTINGS_MENU:
            navigateSettings(1);
            break;
        case EDIT_SETTING:
            adjustSetting(-1);
            break;
        default:
            LOG_DEBUG("No action for DOWN in current state");
            break;
    }
    updateDisplay();
}


void UserInterface::handleNext() {
    switch (current_state_) {
        case MAIN_SCREEN:
            // No action in main screen
            break;
        case SETTINGS_MENU:
            if (current_setting_ < total_num_settings_) {
                enterEditSetting();
            }
            break;
        case EDIT_SETTING:
            // No action in edit setting
            break;
    }
    updateDisplay();
}

void UserInterface::handlePrev() {
    switch (current_state_) {
        case MAIN_SCREEN:
            // No action in main screen
            break;
        case SETTINGS_MENU:
            exitSettingsMenu();
            break;
        case EDIT_SETTING:
            // Discard changes and move back to settings menu
            discardSetting();
            exitEditSetting();
            break;
    }
    updateDisplay();
}

void UserInterface::showMainScreen() {
    lcd_->clear();
    // Display external temperature
    lcd_->setCursor(0, 0);
    lcd_->print("Zewn: ");
    if (!isnan(external_temp_)) {
        lcd_->print(external_temp_);
        lcd_->print(" C");
    } else {
        lcd_->print("Błąd");
    }
    // Display internal temperature
    lcd_->setCursor(0, 1);
    lcd_->print("Wewn: ");
    if (!isnan(internal_temp_)) {
        lcd_->print(internal_temp_);
        lcd_->print(" C");
    } else {
        lcd_->print("Błąd");
    }
    // Display current state
    lcd_->setCursor(14, 0);
    if (is_fan_on_) {
        lcd_->print("Wł");
    }
}

void UserInterface::showSettingsMenu() {
    lcd_->clear();
    lcd_->setCursor(0, 0);
    lcd_->print("Ustawienia:  (");
    lcd_->print(current_setting_ + 1);
    lcd_->print(")");

    lcd_->setCursor(0, 1);
    if (current_setting_ < total_num_settings_) {
        lcd_->print("> ");
        lcd_->print(settings_array_[current_setting_]->getScreenText());
    } else {
        lcd_->print("> Wróc do menu");
    }
}

void UserInterface::showEditSetting() {
    lcd_->clear();
    lcd_->setCursor(0, 0);
    lcd_->print(settings_array_[current_setting_]->getScreenText());

    lcd_->setCursor(0, 1);
    lcd_->print("Wartość: ");
    char value_buffer[5];
    settings_array_[current_setting_]->getValueAsString(value_buffer, sizeof(value_buffer));
    lcd_->print(value_buffer);

    LOG_WARNING("Editing setting: %s", value_buffer);
}

void UserInterface::enterSettingsMenu() {
    current_state_ = SETTINGS_MENU;
    current_setting_ = 0;
}

void UserInterface::exitSettingsMenu() {
    current_state_ = MAIN_SCREEN;
}

void UserInterface::enterEditSetting() {
    current_state_ = EDIT_SETTING;
    if (current_setting_ < total_num_settings_) {
        settings_array_[current_setting_]->loadDataFromPersistence();
    }
}

void UserInterface::exitEditSetting() {
    current_state_ = SETTINGS_MENU;
}

void UserInterface::navigateSettings(int direction) {
    if (current_setting_ + direction <= total_num_settings_) {
        current_setting_ += direction;
        LOG_DEBUG("Navigated to setting %zum dir %d", current_setting_, direction);
    } else {
        LOG_DEBUG("Attempted to navigate out of bounds, current setting: %zu, total settings: %zu, dir: %d", current_setting_, total_num_settings_, direction);
    }
}

void UserInterface::adjustSetting(int delta) {
    if (current_setting_ < total_num_settings_) {
        if (delta > 0) {
            settings_array_[current_setting_]->increase();
        } else if (delta < 0) {
            settings_array_[current_setting_]->decrease();
        }
        LOG_INFO("Adjusted setting: %s by %d", settings_array_[current_setting_]->getName(), delta);
    } else {
        LOG_WARNING("No setting to adjust at index %zu", current_setting_);
    }
    updateDisplay();
}

void UserInterface::saveSetting() {
    if (current_setting_ < total_num_settings_) {
        settings_array_[current_setting_]->save();
        LOG_INFO("Saved setting: %s", settings_array_[current_setting_]->getName());
    } else {
        LOG_WARNING("No setting to save at index %zu", current_setting_);
    }
}

void UserInterface::discardSetting() {
    if (current_setting_ < total_num_settings_) {
        settings_array_[current_setting_]->discard();
        LOG_INFO("Discarded changes for setting: %s", settings_array_[current_setting_]->getName());
    } else {
        LOG_WARNING("No setting to discard at index %zu", current_setting_);
    }
}
