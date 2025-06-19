#include <Arduino.h>    // Essential Arduino header
#include <LiquidCrystal.h>

// LCD Pin Configuration (Standard for Keypad Shields)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // RS, EN, D4, D5, D6, D7
// LiquidCrystal lcd(7, 8,  9, 10, 11, 12);  // RS, EN, D4, D5, D6, D7


// Backlight Control (Typically pin 10)
const int BACKLIGHT_PIN = 10; 

// Button Analog Pin
const int BUTTON_PIN = A0;

void setup() {
  // Initialize backlight control
  pinMode(BACKLIGHT_PIN, OUTPUT);
  digitalWrite(BACKLIGHT_PIN, HIGH);  // Start with backlight ON
  
  // Initialize LCD
  lcd.begin(16, 2);                   // Set dimensions (16x2)
  lcd.setCursor(0,0);
  lcd.print("Display Active!");        // Test message
  delay(1000);                         // Show initial message 
}

void loop() {
  // Turn off backlight after 5 seconds of inactivity
  static unsigned long lastActive = millis();
  
  if (analogRead(BUTTON_PIN) < 1000) {  // If any button pressed
    lastActive = millis();
    digitalWrite(BACKLIGHT_PIN, HIGH);  // Ensure backlight ON
    lcd.clear();
    lcd.print("Button pressed!");
    delay(500);
  }
  
  // Auto-backlight off after 5s
  if (millis() - lastActive > 5000) {
    digitalWrite(BACKLIGHT_PIN, LOW);
    lcd.clear();
    // lcd.noDisplay();  // Additional power saving
  }
}