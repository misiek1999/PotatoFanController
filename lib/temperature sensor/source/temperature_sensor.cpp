#include "temperature_sensor.h"

#include <log.h>

constexpr auto kMaxTempRes = 12U; // Maximum resolution for DS18B20 (12 bits)

Sensor::TemperatureSensor::TemperatureSensor(uint8_t pin):
    _pin(pin),
    _one_wire(pin),
    _sensor(&_one_wire) {
    LOG_INFO("TemperatureSensor initialized on pin %d", pin);
}

void Sensor::TemperatureSensor::begin() {
    _sensor.begin();
    LOG_INFO("Temperature sensor on pin %d started", _pin);
    _sensor.setResolution(kMaxTempRes);
    LOG_DEBUG("Temperature sensor on pin %d set to resolution %d bits", _pin, kMaxTempRes);
}

bool Sensor::TemperatureSensor::isConnected() noexcept {
    bool connected = _sensor.getDeviceCount() > 0;
    LOG_VERBOSE("Temperature sensor on pin %d is %sconnected", _pin, connected ? "" : "NOT ");
    return connected;
}

float Sensor::TemperatureSensor::readTemperature() noexcept {
    if (!isConnected()) {
        LOG_WARNING("Temperature sensor on pin %d is not connected", _pin);
        return NAN; // Return NaN if sensor is not connected
    }

    _sensor.requestTemperatures();
    float temperature = _sensor.getTempCByIndex(0); // Get temperature in Celsius
    LOG_VERBOSE("Temperature read from sensor on pin %d: %F °C", _pin, temperature);
    return temperature;
}
