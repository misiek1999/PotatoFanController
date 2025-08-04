#pragma once

#include <Arduino.h>

class ISetting {
public:
    virtual ~ISetting() = default;
    virtual void increase() = 0; // Apply the setting
    virtual void decrease() = 0; // Revert the setting
    virtual const char* getName() const = 0; // Get the name of the setting
    virtual bool save() const = 0; // Save the setting value
    virtual void discard() = 0; // Discard changes
    virtual void getDescription(char* buffer, size_t buffer_size) = 0; // Get a description of the setting, which might be used for display
};

template<typename T, size_t kNameMaxLen = 16>
class Setting : public ISetting {
    using GetValueFuncType = T(*)();
    using SetValueFuncType = void(*)(T);
public:
    /**
     * @param name         The name of the setting
     * @param defaultValue Value to reset to
     * @param step         Increment/decrement step
     */
    Setting(const char* name, GetValueFuncType get_value_func,
            SetValueFuncType set_value_func, T step = static_cast<T>(1));

    // ISetting interface
    void increase() override;
    void decrease() override;
    const char* getName() const override;
    bool save() const override;
    void discard() override;
    void getDescription(char* buffer, size_t buffer_size) override;

private:
    char name_[kNameMaxLen];
    GetValueFuncType get_value_func_; // Function to get the current value
    SetValueFuncType set_value_func_; // Function to set the value
    T           value_;
    mutable T   savedValue_;
    T           step_;
};
