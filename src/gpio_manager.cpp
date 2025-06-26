#include "gpio_manager.h"
#include "gpio_hal.h"  // Include GPIO hal header
#include "log.h"

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

    bool isButtonPressed(uint16_t buttonPin) {
        const auto result = HAL::isButtonPressed(buttonPin);
        LOG_DEBUG("Checking if button on pin %d is pressed: %s", buttonPin, result ? "YES" : "NO");
        return result;
    }

    bool isKeypadNextPressed() {
        const auto result = HAL::isKeypadNextPressed();
        LOG_DEBUG("Checking if keypad next button is pressed: %s", result ? "YES" : "NO");
        return result;
    }

    bool isKeypadPrevPressed() {
        const auto result = HAL::isKeypadPrevPressed();
        LOG_DEBUG("Checking if keypad previous button is pressed: %s", result ? "YES" : "NO");
        return result;
    }

    bool isKeypadSelectPressed() {
        const auto result = HAL::isKeypadSelectPressed();
        LOG_DEBUG("Checking if keypad select button is pressed: %s", result ? "YES" : "NO");
        return result;
    }

    bool isKeypadUpPressed() {
        const auto result = HAL::isKeypadUpPressed();
        LOG_DEBUG("Checking if keypad up button is pressed: %s", result ? "YES" : "NO");
        return result;
    }

    bool isKeypadDownPressed() {
        const auto result = HAL::isKeypadDownPressed();
        LOG_DEBUG("Checking if keypad down button is pressed: %s", result ? "YES" : "NO");
        return result;
    }
} // namespace GPIO
