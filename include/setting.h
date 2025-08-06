#pragma once

#include <Arduino.h>
#include "log.h"
#include "type_traits_arduino.h"
#include <stdio.h>

/**
 * Interface for a configurable setting.
 */
class ISetting {
public:
    // we dont need a virtual destructor here
    virtual void increase() = 0;
    virtual void decrease() = 0;
    virtual const char* getName() const = 0;
    virtual bool save() const = 0;
    virtual void discard() = 0;
    virtual void getDescription(char* buffer, size_t buffer_size) = 0;
    virtual void getValueAsString(char* buffer, size_t buffer_size) const = 0;
    virtual const char* getScreenText() const = 0; // For displaying on the screen
};

/**
 * @tparam T            Type of the setting value (e.g. float, int)
 * @tparam kNameMaxLen  Maximum length of the setting name (including null terminator)
 *
 * A header-only template for a single setting, implementing ISetting.
 */
template<typename T, size_t kNameMaxLen = 16>
class Setting : public ISetting {
    static_assert(kNameMaxLen > 0, "kNameMaxLen must be greater than 0");
    using GetValueFuncType = T(*)();
    using SetValueFuncType = void(*)(T);

public:
    /**
     * @param name              Human-readable name (must be shorter than kNameMaxLen)
     * @param get_value_func    Function to read the current stored value
     * @param set_value_func    Function to persist a new value
     * @param step              Increment/decrement step (default = 1)
     */
    Setting(const char* name,
            const char* screen_text,
            GetValueFuncType get_value_func,
            SetValueFuncType set_value_func,
            T step = static_cast<T>(1))
      : get_value_func_{get_value_func}
      , set_value_func_{set_value_func}
      , step_{step}
    {
        if (strlen(name) >= kNameMaxLen) {
            LOG_FATAL("Setting name exceeds maximum length");
        }
        strncpy(name_, name, kNameMaxLen);
        if (strlen(screen_text) >= kNameMaxLen) {
            LOG_FATAL("Setting screen text exceeds maximum length");
        }
        strncpy(screen_text_, screen_text, kNameMaxLen);

        name_[kNameMaxLen - 1] = '\0';
        screen_text_[kNameMaxLen - 1] = '\0';

        // Initialize from storage
        value_      = get_value_func_();
        savedValue_ = value_;
    }

    // — ISetting interface —
    void increase() override {
        value_ += step_;
    }

    void decrease() override {
        value_ -= step_;
    }

    const char* getName() const override {
        return name_;
    }

    const char* getScreenText() const override {
        return screen_text_;
    }

    void getValueAsString(char* buffer, size_t buffer_size) const override {
        if constexpr (gpt::is_integral_v<T>) {
            snprintf(buffer, buffer_size, "%d", value_);
        } else if constexpr (gpt::is_floating_point_v<T>) {
            (void) buffer_size; // Unused in this case
            dtostrf(value_, 2, 1, buffer); // Format float with 2 decimal places
        } else {
            snprintf(buffer, buffer_size, "Unknown");
        }
    }

    bool save() const override {
        set_value_func_(value_);
        // after persisting, update savedValue_
        savedValue_ = value_;
        return true;
    }

    void discard() override {
        value_ = savedValue_;
    }

    void getDescription(char* buffer, size_t buffer_size) override {
        if constexpr (gpt::is_integral_v<T>) {
            snprintf(buffer, buffer_size, "%s: %d", screen_text_, value_);
        }
        else if constexpr (gpt::is_floating_point_v<T>) {
            (void) buffer_size; // Unused in this case
            dtostrf(value_, 2, 1, buffer); // Format float with 2 decimal places
        }
        else {
            snprintf(buffer, buffer_size, "Unsupported type");
        }
    }

private:
    char              name_[kNameMaxLen];
    char              screen_text_[kNameMaxLen];
    GetValueFuncType  get_value_func_;
    SetValueFuncType  set_value_func_;
    T                 value_;
    mutable T         savedValue_;
    T                 step_;
};
