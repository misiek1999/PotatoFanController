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
        static constexpr size_t kFilterSize = 10;  // moving‐average window

        uint8_t _pin;
        OneWire _one_wire;
        DallasTemperature _sensor;

        // moving‐average filter state
        float _buffer[kFilterSize] = { 0 };
        size_t _bufIndex     = 0;
        size_t _bufCount     = 0;
        float  _sum          = 0.0f;

        void loadIntoFilter(float sample);
    };
} // namespace  Sensor
