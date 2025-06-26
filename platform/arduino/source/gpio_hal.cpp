#include "gpio_hal.h"
#include "project_pin_definition.h"

#include <Arduino.h>  // Include Arduino library for pin manipulation

namespace HAL {

    bool initGPIO() {
        // Initialize GPIO pins here
        pinMode(KEYPAD_ANALOG_BUTTON_PIN, INPUT);
        pinMode(RELAY_PIN, OUTPUT);

        // Set digital button pins as inputs
        pinMode(BEFORE_BUTTON_PIN, INPUT_PULLUP);
        pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
        pinMode(NEXT_BUTTON_PIN, INPUT_PULLUP);
        pinMode(INCREASE_BUTTON_PIN, INPUT_PULLUP);
        pinMode(DECREASE_BUTTON_PIN, INPUT_PULLUP);

        return true;  // Return true if initialization is successful
    }

    void setRelay(bool state) {
        // Set relay pin state
        // Assuming relay is connected to a digital pin, e.g., pin 12
        digitalWrite(RELAY_PIN, state ? HIGH : LOW);
    }

    void setBacklight(bool state) {
        // Set backlight pin state
        // Assuming backlight is connected to a digital pin, e.g., pin 10
        digitalWrite(LCD_BACKLIGHT_PIN, state ? HIGH : LOW);
    }

    bool isButtonPressed(uint16_t buttonPin) {
        // Check if the button is pressed
        // Assuming active low logic for buttons
        return digitalRead(buttonPin) == LOW;
    }

    bool isButtonHolded(uint16_t buttonPin) {
        // Check if the button is holded
        // Assuming active low logic for buttons
        // TODO: Implement hold detection logic if needed
        return digitalRead(buttonPin) == LOW;
    }
    #ifndef USE_ANALOG_KEYPAD
    bool isKeypadNextPressed() {
        // Check if the keypad next button is pressed
        return isButtonPressed(NEXT_BUTTON_PIN);
    }

    bool isKeypadPrevPressed() {
        // Check if the keypad previous button is pressed
        return isButtonPressed(BEFORE_BUTTON_PIN);
    }

    bool isKeypadSelectPressed() {
        // Check if the keypad select button is pressed
        return isButtonPressed(SELECT_BUTTON_PIN);
    }

    bool isKeypadUpPressed() {
        // Check if the keypad up button is pressed
        return isButtonPressed(INCREASE_BUTTON_PIN);
    }

    bool isKeypadDownPressed() {
        // Check if the keypad down button is pressed
        return isButtonPressed(DECREASE_BUTTON_PIN);
    }
    #else

    int getKeypadAnalogValue() {
        // Read the analog value from the keypad button pin
        return analogRead(KEYPAD_ANALOG_BUTTON_PIN);
    }

    bool isKeypadNextPressed() {
        // Check if the keypad next button is pressed
        const auto analogKeypadKey = analogRead(KEYPAD_ANALOG_BUTTON_PIN);
        return analogKeypadKey < 50;
    }

    bool isKeypadPrevPressed() {
        // Check if the keypad previous button is pressed
        const auto analogKeypadKey = analogRead(KEYPAD_ANALOG_BUTTON_PIN);
        return analogKeypadKey >= 300 && analogKeypadKey < 500;
    }

    bool isKeypadSelectPressed() {
        // Check if the keypad select button is pressed
        const auto analogKeypadKey = analogRead(KEYPAD_ANALOG_BUTTON_PIN);
        return analogKeypadKey >= 500 && analogKeypadKey < 750;
    }

    bool isKeypadUpPressed() {
        // Check if the keypad up button is pressed
        const auto analogKeypadKey = analogRead(KEYPAD_ANALOG_BUTTON_PIN);
        return analogKeypadKey >= 50 && analogKeypadKey < 150;
    }

    bool isKeypadDownPressed() {
        const auto analogKeypadKey = analogRead(KEYPAD_ANALOG_BUTTON_PIN);
        // Check if the keypad down button is pressed
        return analogKeypadKey >= 150 && analogKeypadKey < 300;
    }
    #endif  // USE_ANALOG_KEYPAD

}   // namespace HAL
