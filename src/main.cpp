#include <math.h> // For NAN
#include <Arduino.h>    // Essential Arduino header
#include <LiquidCrystal.h>
#include "liquid_crystal_ext.h" // Extended LiquidCrystal library for Polish characters
#include "log.h"       // Logging library
#include "project_pin_definition.h" // Pin definitions for the project
#include "pin_duplication_check.h" // Pin duplication check
#include "temperature_sensor.h" // Temperature sensor library
#include "gpio_manager.h"
#include "persistence_manager.h"
#include "persistence_manager_instance.h" // Singleton instance of PersistenceManager
#include "user_interface.h" // User interface controller

// DS18B20 sensors and temp readings
Sensor::TemperatureSensor external_sensor(EXTERNAL_DS18B20_PIN); // Initialize temperature sensor on external sensor pin
Sensor::TemperatureSensor internal_sensor(INTERNAL_DS18B20_PIN); // Initialize temperature sensor on internal sensor pin
float external_temp = NAN; // Variable to hold external temperature
float internal_temp = NAN; // Variable to hold internal temperature

// LCD with Polish characters support
PolishLCD lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);  // RS, EN, D4, D5, D6, D7
// Function to get the singleton instance of UserInterface
UserInterface& getUIInstance() {
    static UserInterface instance(&lcd); // Create a singleton instance of UserInterface
    return instance;
}

// Variable to hold last loop time
auto last_main_loop_time = 0UL; // Variable to track the last loop time

// variable to save fan stats
bool fan_active = false;
unsigned long last_fan_change_time = 0;

// temperature sensor selector
size_t selected_temp_sens = 0;

void setup() {
    // Initialize Serial for logging
    Serial.begin(115200);
    while (!Serial);  // Wait for Serial to be ready
    initLog();        // Initialize the logging system

    //initialize GPIO pins
    if (!GPIO::initGPIO()) {
        LOG_FATAL("Failed to initialize GPIO pins!");
    }
    // Initialize the temperature sensor
    external_sensor.begin();
    internal_sensor.begin();

    // Initialize the LCD
    lcd.beginPolish(16, 2); // Initialize LCD with Polish characters support
    lcd.begin(16, 2);                   // Set dimensions (16x2)
    lcd.setCursor(0,0);
    lcd.print("Aktywacja!");        // Test message

    // Initialize the persistence manager
    (void) getPersistenceManagerInstance();
    // Initialize the user interface controller
    (void) getUIInstance();            // Initialize the UI controller

    // update last fan change state time
    last_fan_change_time = millis();

    LOG_INFO("Setup completed");
}

void loop() {
    last_main_loop_time = millis(); // Update the last main loop time
    switch(selected_temp_sens++) {
    case 0:
        if (external_sensor.isConnected()) {
            external_temp = external_sensor.readTemperature();
        } else {
            external_temp = NAN; // Set to NAN if sensor is not connected
        }
        break;
    case 1:
        if (internal_sensor.isConnected()) {
            internal_temp = internal_sensor.readTemperature();
        } else {
            internal_temp = NAN; // Set to NAN if sensor is not connected
        }
        selected_temp_sens = 0;
        break;
    default: break;
    }

    // Control fan
    do {
        const auto* const persi_manager = getPersistenceManagerInstance();
        const auto min_ext_temp = persi_manager->getMinimalExternalTemperature();
        const auto max_ext_temp = persi_manager->getMaximalExternalTemperature();
        const auto switch_time = persi_manager->getSwitchTimeHysteresis();
        const auto temp_diff = persi_manager->getTemperatureDifferenceHysteresis();

        // 1) time‐hysteresis: if we switched too recently, ignore.
        if (last_main_loop_time - last_fan_change_time < switch_time) {
            break;
        }

        // 2) temperature‐hysteresis & range checks
        if (!fan_active) {
            // Off → On
            if (external_temp > min_ext_temp &&
                external_temp < max_ext_temp &&
                external_temp < internal_temp - temp_diff) {
                fan_active      = true;
            }
        }
        else {
            // On → Off
            if (external_temp <= min_ext_temp ||
                external_temp >= max_ext_temp ||
                external_temp > internal_temp + temp_diff) {
                fan_active      = false;
            }
        }
        last_fan_change_time = last_main_loop_time;
    } while (0);

    LOG_DEBUG("Ext temp: %F, Int temp: %F, Fan: %d", external_temp, internal_temp, fan_active);

    // Get the singleton instance of UserInterface
    UserInterface& userInterface = getUIInstance();

    // Update the user interface with the latest temperature readings
    userInterface.setExternalTemperature(external_temp);
    userInterface.setInternalTemperature(internal_temp);
    userInterface.setFanState(fan_active);

    // Poll user inputs
    if (GPIO::isKeypadSelectPressed()) {
        userInterface.handleSelect();  // Handle select button press
    }
    if (GPIO::isKeypadUpPressed()) {
        userInterface.handleUp();      // Handle up button press
    }
    if (GPIO::isKeypadDownPressed()) {
        userInterface.handleDown();    // Handle down button press
    }
    if (GPIO::isKeypadNextPressed()) {
        userInterface.handleNext();    // Handle next button press
    }
    if (GPIO::isKeypadPrevPressed()) {
        userInterface.handlePrev();    // Handle previous button press
    }
    userInterface.updateDisplay();  // Update the display based on the current state

    const auto current_time = millis();
    LOG_DEBUG("Main loop duration %l ms", current_time - last_main_loop_time);
    delay(200);  // Delay to prevent excessive CPU usage
}
