#include "gpio_manager.h"
#include "gpio_hal.h"  // Include GPIO hal header
#include "log.h"

#include <Arduino.h>

#define MAX_TRACKED_BUTTONS 8

namespace GPIO {

    bool initGPIO() {
        // Initialize GPIO pins here
        // For example, set pin modes, initialize relays, etc.
        LOG_INFO("GPIO initialization started");
        const auto result = HAL::initGPIO();
        LOG_INFO("GPIO initialization completed %s", result ? "success" : "failure");
        return result;  // Return true if initialization is successful
    }

    void setRelay(bool state) {
        LOG_INFO("Setting relay to %s", state ? "ON" : "OFF");
        HAL::setRelay(state);
    }

    void setBacklight(bool state) {
        LOG_INFO("Setting backlight to %s", state ? "ON" : "OFF");
        HAL::setBacklight(state);
    }

    bool isButtonActive(uint16_t buttonPin) {
        const auto result = HAL::isButtonPressed(buttonPin);
        LOG_DEBUG("Checking if button on pin %d is pressed: %s", buttonPin, result ? "YES" : "NO");
        return result;
    }

    // For generic button pins
    bool isButtonPressed(uint16_t buttonPin) {
        constexpr uint32_t DEBOUNCE_MS = 50;   // adjust as needed

        static uint16_t  trackedPins[MAX_TRACKED_BUTTONS]    = {0};
        static bool      lastStates[MAX_TRACKED_BUTTONS]     = {false};
        static uint32_t  lastTimestamps[MAX_TRACKED_BUTTONS] = {0};
        static uint8_t   trackedCount                       = 0;

        // find or add this pin in our arrays
        uint8_t index = 0;
        for (; index < trackedCount; ++index) {
            if (trackedPins[index] == buttonPin) {
                break;
            }
        }
        if (index == trackedCount) {
            if (trackedCount >= MAX_TRACKED_BUTTONS) {
                LOG_ERROR("Max button tracking reached for pin: %d", buttonPin);
                exit(1);
            }
            trackedPins[trackedCount]    = buttonPin;
            lastStates[trackedCount]     = HAL::isButtonPressed(buttonPin);
            lastTimestamps[trackedCount] = millis();
            ++trackedCount;
        }

        // read current state
        bool    currentState = HAL::isButtonPressed(buttonPin);
        bool    risingEdge   = currentState && !lastStates[index];
        uint32_t now         = millis();

        // debounce: only treat as a valid press if enough time has passed
        bool debouncedPress = false;
        if (risingEdge) {
            if ((now - lastTimestamps[index]) >= DEBOUNCE_MS) {
                debouncedPress = true;
                lastTimestamps[index] = now;
            }
        }

        // update state for next call
        lastStates[index] = currentState;

        LOG_VERBOSE("Button on pin %d: %s (raw rising: %s, debounced: %s)",
                    buttonPin,
                    currentState   ? "HIGH" : "LOW",
                    risingEdge     ? "YES" : "NO",
                    debouncedPress ? "YES" : "NO");

        return debouncedPress;
    }

    // For specific keypad buttons
    bool isKeypadNextPressed() {
        static bool lastState = false;
        const bool currentState = HAL::isKeypadNextPressed();
        const bool risingEdge = currentState && !lastState;
        lastState = currentState;

        LOG_VERBOSE("Keypad Next: %s (rising edge: %s)",
                currentState ? "HIGH" : "LOW",
                risingEdge ? "YES" : "NO");

        return risingEdge;
    }

    bool isKeypadPrevPressed() {
        static bool lastState = false;
        const bool currentState = HAL::isKeypadPrevPressed();
        const bool risingEdge = currentState && !lastState;
        lastState = currentState;

        LOG_VERBOSE("Keypad Prev: %s (rising edge: %s)",
                currentState ? "HIGH" : "LOW",
                risingEdge ? "YES" : "NO");

        return risingEdge;
    }

    bool isKeypadSelectPressed() {
        static bool lastState = false;
        const bool currentState = HAL::isKeypadSelectPressed();
        const bool risingEdge = currentState && !lastState;
        lastState = currentState;

        LOG_VERBOSE("Keypad Select: %s (rising edge: %s)",
                currentState ? "HIGH" : "LOW",
                risingEdge ? "YES" : "NO");

        return risingEdge;
    }

    bool isKeypadUpPressed() {
        static bool lastState = false;
        const bool currentState = HAL::isKeypadUpPressed();
        const bool risingEdge = currentState && !lastState;
        lastState = currentState;

        LOG_VERBOSE("Keypad Up: %s (rising edge: %s)",
                currentState ? "HIGH" : "LOW",
                risingEdge ? "YES" : "NO");

        return risingEdge;
    }

    bool isKeypadDownPressed() {
        static bool lastState = false;
        const bool currentState = HAL::isKeypadDownPressed();
        const bool risingEdge = currentState && !lastState;
        lastState = currentState;

        LOG_VERBOSE("Keypad Down: %s (rising edge: %s)",
                currentState ? "HIGH" : "LOW",
                risingEdge ? "YES" : "NO");

        return risingEdge;
    }
}  // namespace GPIO
