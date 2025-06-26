#pragma once

#include <stdint.h>

namespace HAL
{
    bool initGPIO();

    void setRelay(bool state);

    void setBacklight(bool state);

    bool isButtonPressed(uint16_t buttonPin);

    bool isButtonHolded(uint16_t buttonPin);

    bool isKeypadNextPressed();

    bool isKeypadPrevPressed();

    bool isKeypadSelectPressed();

    bool isKeypadUpPressed();

    bool isKeypadDownPressed();

} // namespace HAL
