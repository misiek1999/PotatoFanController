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
#include "user_interface.h" // User interface controller

// DS18B20 sensors and temp readings
Sensor::TemperatureSensor external_sensor(EXTERNAL_DS18B20_PIN); // Initialize temperature sensor on external sensor pin
Sensor::TemperatureSensor internal_sensor(INTERNAL_DS18B20_PIN); // Initialize temperature sensor on internal sensor pin
float external_temp = NAN; // Variable to hold external temperature
float internal_temp = NAN; // Variable to hold internal temperature

// Variables to hold settings
PersistenceManager* persistence_manager; // Persistence manager for settings
PersistenceManager* getSingletonPersistenceManager() {
    static PersistenceManager instance; // Create a singleton instance of PersistenceManager
    return &instance;
}

// LCD with Polish characters support
PolishLCD lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);  // RS, EN, D4, D5, D6, D7
// Function to get the singleton instance of UserInterface
UserInterface& getSingletonUI() {
    const auto* persistence_manager = getSingletonPersistenceManager(); // Get the singleton instance of PersistenceManager
    static UserInterface instance(&lcd, persistence_manager); // Create a singleton instance of UserInterface
    return instance;
}

// Variable to hold last loop time
auto last_main_loop_time = 0UL; // Variable to track the last loop time

void setup() {
    // Initialize Serial for logging
    Serial.begin(115200);
    while (!Serial);  // Wait for Serial to be ready
    initLog();        // Initialize the logging system

    //initialize GPIO pins
    if (!GPIO::initGPIO()) {
        LOG_ERROR("Failed to initialize GPIO pins!");
        return; // Exit setup if GPIO initialization fails
    }
    // Initialize the temperature sensor
    external_sensor.begin();
    internal_sensor.begin();

    // Initialize the LCD
    lcd.begin(16, 2);                   // Set dimensions (16x2)
    lcd.setCursor(0,0);
    lcd.print("Display Active!");        // Test message

    // Initialize the user interface controller
    (void) getSingletonUI();            // Initialize the UI controller

    // Initialize the persistence manager
    (void) getSingletonPersistenceManager();
    const auto data = getSingletonPersistenceManager()->getSwitchTimeHysteresis();
    LOG_INFO("Switch time hysteresis: %l ms", (long)data);

    LOG_INFO("Setup completed successfully");
}

void loop() {
    last_main_loop_time = millis(); // Update the last main loop time
    if (external_sensor.isConnected()) {
        external_temp = external_sensor.readTemperature();
        LOG_INFO("Current temperature: %F °C", external_temp);
    } else {
        external_temp = NAN; // Set to NAN if sensor is not connected
        LOG_ERROR("Temperature sensor not connected!");
    }
    if (internal_sensor.isConnected()) {
        internal_temp = internal_sensor.readTemperature();
        LOG_INFO("Internal temperature: %F °C", internal_temp);
    } else {
        internal_temp = NAN; // Set to NAN if sensor is not connected
        LOG_ERROR("Internal temperature sensor not connected!");
    }
    // Get the singleton instance of UserInterface
    UserInterface& userInterface = getSingletonUI();

    // Update the user interface with the latest temperature readings
    userInterface.setExternalTemperature(external_temp);
    userInterface.setInternalTemperature(internal_temp);

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
