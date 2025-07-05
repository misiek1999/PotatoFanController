#pragma once
#include <Arduino.h>

// DS18B20
constexpr auto EXTERNAL_DS18B20_PIN = 2U;   // Pin for external DS18B20 sensor
constexpr auto INTERNAL_DS18B20_PIN = 3U;   // Pin for internal DS18B20 sensor

// LCD Display
constexpr auto LCD_RS_PIN           = 8U;   // Register Select pin for LCD
constexpr auto LCD_EN_PIN           = 9U;   // Enable pin for LCD
constexpr auto LCD_D4_PIN           = 4U;   // Data pin D4 for LCD
constexpr auto LCD_D5_PIN           = 5U;   // Data pin D5 for LCD
constexpr auto LCD_D6_PIN           = 6U;   // Data pin D6 for LCD
constexpr auto LCD_D7_PIN           = 7U;   // Data pin D7 for LCD
constexpr auto LCD_BACKLIGHT_PIN    = 10U;  // Backlight pin for LCD

// Analog button
constexpr auto KEYPAD_ANALOG_BUTTON_PIN    = A0;   // Analog pin for button input

// Digital pins
constexpr auto SELECT_BUTTON_PIN    = A1;   // Select button pin
constexpr auto DECREASE_BUTTON_PIN  = A2;   // Decrease button pin
constexpr auto INCREASE_BUTTON_PIN  = A3;   // Increase button pin
constexpr auto BEFORE_BUTTON_PIN    = A4;   // Before button pin
constexpr auto NEXT_BUTTON_PIN      = A5;   // Next button pin

// Relay control
constexpr auto RELAY_PIN            = 12U;  // Pin for relay control
