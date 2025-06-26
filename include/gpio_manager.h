#pragma once

#include <stdint.h>

namespace GPIO {
    bool initGPIO();

    void setRelay(bool state);

    void setBacklight(bool state);

    bool isButtonPressed(uint16_t buttonPin);

    bool isKeypadNextPressed();

    bool isKeypadPrevPressed();

    bool isKeypadSelectPressed();

    bool isKeypadUpPressed();

    bool isKeypadDownPressed();

} // GPIO manager methods
