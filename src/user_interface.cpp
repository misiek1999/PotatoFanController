#include "user_interface.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <liquid_crystal_ext.h>
#include "log.h"

UserInterface::UserInterface(PolishLCD* lcd)
    : lcd_(lcd), current_state_(MAIN_SCREEN), current_index_(0) {
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
            if (current_index_ < num_settings_) {
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
            navigateSettings(1);
            break;
        case EDIT_SETTING:
            // Save and move to next setting
            saveSetting();
            if (current_index_ < num_settings_ - 1) {
                current_index_++;
                showEditSetting();
            } else {
                exitEditSetting();
            }
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
            navigateSettings(-1);
            break;
        case EDIT_SETTING:
            // Save and move to previous setting
            saveSetting();
            if (current_index_ > 0) {
                current_index_--;
                showEditSetting();
            } else {
                exitEditSetting();
            }
            break;
    }
    updateDisplay();
}

void UserInterface::addSetting(const char* name, int* value_ptr, int min_val,
                           int max_val) {
    if (num_settings_ < MAX_SETTINGS) {
        settings_[num_settings_] = {name, value_ptr, min_val, max_val};
        num_settings_++;
    }
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
        lcd_->print("Blad");
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
    lcd_->print("Ustawienia:");

    lcd_->setCursor(0, 1);
    if (current_index_ < num_settings_) {
        lcd_->print(settings_[current_index_].name);
    } else {
        lcd_->print("> Wróć");
    }
}

void UserInterface::showEditSetting() {
    lcd_->clear();
    lcd_->setCursor(0, 0);
    lcd_->print("Wartość: ");
    lcd_->print(settings_[current_index_].name);

    lcd_->setCursor(0, 1);
    lcd_->print("Wartość: ");
    lcd_->print(*(settings_[current_index_].value_ptr));
}

void UserInterface::enterSettingsMenu() {
    current_state_ = SETTINGS_MENU;
    current_index_ = 0;
}

void UserInterface::exitSettingsMenu() {
    current_state_ = MAIN_SCREEN;
}

void UserInterface::enterEditSetting() {
    current_state_ = EDIT_SETTING;
}

void UserInterface::exitEditSetting() {
    current_state_ = SETTINGS_MENU;
}

void UserInterface::navigateSettings(int direction) {
    current_index_ += direction;
    if (current_index_ < 0) current_index_ = num_settings_;
    if (current_index_ > num_settings_) current_index_ = 0;
}

void UserInterface::adjustSetting(int delta) {
    Setting& s = settings_[current_index_];
    *(s.value_ptr) = constrain(*(s.value_ptr) + delta, s.min_val, s.max_val);
    updateDisplay();
}

void UserInterface::saveSetting() {
    // Placeholder for saving to EEPROM if needed
    // EEPROM.put(address, *(settings_[current_index_].value_ptr));
}