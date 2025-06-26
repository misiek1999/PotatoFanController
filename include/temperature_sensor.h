#pragma once

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

namespace  Sensor
{
    class TemperatureSensor
    {
    public:
        TemperatureSensor(uint8_t pin);
        void begin();
        bool isConnected() noexcept;
        float readTemperature() noexcept;

    private:
        uint8_t _pin;
        OneWire _one_wire;
        DallasTemperature _sensor;
    };
} // namespace  Sensor
