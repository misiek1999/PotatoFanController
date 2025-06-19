#include <Arduino.h>    // Essential Arduino header
#include <LiquidCrystal.h>
#include "log.h"       // Logging library
#include "project_pin_definition.h" // Pin definitions for the project
#include "temperature_sensor.h" // Temperature sensor library

Sensor::TemperatureSensor external_sensor(EXTERNAL_DS18B20_PIN); // Initialize temperature sensor on external sensor pin
Sensor::TemperatureSensor internal_sensor(INTERNAL_DS18B20_PIN); // Initialize temperature sensor on internal sensor pin

LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);  // RS, EN, D4, D5, D6, D7


void setup() {
    // Initialize Serial for logging
    Serial.begin(115200);
    while (!Serial);  // Wait for Serial to be ready
    initLog();        // Initialize the logging system

    // Initialize the temperature sensor
    external_sensor.begin();
    internal_sensor.begin();
}

void loop() {
    // Example log messages
    LOG_VERBOSE("Verbose log message");

    // Simulate some operations
    for (int i = 0; i < 5; ++i) {
        LOG_NOTICE("Iteration %d", i);
        if (external_sensor.isConnected()) {
            float temperature = external_sensor.readTemperature();
            LOG_INFO("Current temperature: %F °C", temperature);
        } else {
            LOG_ERROR("Temperature sensor not connected!");
        }
        if (internal_sensor.isConnected()) {
            float internal_temp = internal_sensor.readTemperature();
            LOG_INFO("Internal temperature: %F °C", internal_temp);
        } else {
            LOG_ERROR("Internal temperature sensor not connected!");
        }
        delay(1000);  // Wait for a second
    }
}


// // // LCD Pin Configuration (Standard for Keypad Shields)
// LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // RS, EN, D4, D5, D6, D7
// // LiquidCrystal lcd(7, 8,  9, 10, 11, 12);  // RS, EN, D4, D5, D6, D7


// // Backlight Control (Typically pin 10)
// const int BACKLIGHT_PIN = 10;

// // Button Analog Pin
// const int BUTTON_PIN = A0;

// void setup() {
//   // Initialize backlight control
//   pinMode(BACKLIGHT_PIN, OUTPUT);
//   digitalWrite(BACKLIGHT_PIN, HIGH);  // Start with backlight ON

//   // Initialize LCD
//   lcd.begin(16, 2);                   // Set dimensions (16x2)
//   lcd.setCursor(0,0);
//   lcd.print("Display Active!");        // Test message
//   delay(1000);                         // Show initial message
// }

// void loop() {
//   // Turn off backlight after 5 seconds of inactivity
//   static unsigned long lastActive = millis();

//   if (analogRead(BUTTON_PIN) < 1000) {  // If any button pressed
//     lastActive = millis();
//     digitalWrite(BACKLIGHT_PIN, HIGH);  // Ensure backlight ON
//     lcd.clear();
//     lcd.print("Button pressed!");
//     delay(500);
//   }

//   // Auto-backlight off after 5s
//   if (millis() - lastActive > 5000) {
//     digitalWrite(BACKLIGHT_PIN, LOW);
//     lcd.clear();
//     // lcd.noDisplay();  // Additional power saving
//   }
// }