#include "temperature_sensor.h"

#include "log.h"

constexpr auto kMaxTempRes = 12U; // Maximum resolution for DS18B20 (12 bits)

Sensor::TemperatureSensor::TemperatureSensor(uint8_t pin):
    _pin(pin),
    _one_wire(pin),
    _sensor(&_one_wire) {
    LOG_INFO("TemperatureSensor initialized on pin %d", pin);
}

void Sensor::TemperatureSensor::begin() {
    _sensor.begin();
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
    float raw = _sensor.getTempCByIndex(0);
    LOG_VERBOSE("Raw temperature read from pin %d: %F °C", _pin, raw);

    // Load into moving‐average filter
    loadIntoFilter(raw);
    float average = _sum / static_cast<float>(_bufCount);
    LOG_VERBOSE("Filtered (mean) temperature: %F °C", average);

    return average;
}


void Sensor::TemperatureSensor::loadIntoFilter(float sample) {
    // subtract oldest value from sum
    if (_bufCount == kFilterSize) {
        _sum -= _buffer[_bufIndex];
    } else {
        _bufCount++;
    }

    // store new sample
    _buffer[_bufIndex] = sample;
    _sum += sample;

    // advance index (wrapping around)
    _bufIndex = ( _bufIndex + 1 ) % kFilterSize;
}
