#pragma once

#include "project_pin_definition.h"


// Compile-time duplicate pin value check
namespace pin_check {
    // Helper templates for compile-time duplicate detection
    template <typename T, T... Args>
    struct Array {
        static constexpr T values[] = {Args...};
    };

    template <typename T, T... Args>
    constexpr T Array<T, Args...>::values[];

    template <typename T, T a, T... rest>
    struct Contains {
        static constexpr bool value = false;
    };

    template <typename T, T a, T first, T... rest>
    struct Contains<T, a, first, rest...> {
        static constexpr bool value = (a == first) || Contains<T, a, rest...>::value;
    };

    template <typename T, T... Args>
    struct CheckDuplicates {
        static constexpr bool value = false;
    };

    template <typename T, T first, T... rest>
    struct CheckDuplicates<T, first, rest...> {
        static constexpr bool value = Contains<T, first, rest...>::value ||
                                      CheckDuplicates<T, rest...>::value;
    };
}

// Define pin values as template parameters
using PinArray = pin_check::Array<unsigned,
    EXTERNAL_DS18B20_PIN,
    INTERNAL_DS18B20_PIN,
    LCD_RS_PIN,
    LCD_EN_PIN,
    LCD_D4_PIN,
    LCD_D5_PIN,
    LCD_D6_PIN,
    LCD_D7_PIN,
    LCD_BACKLIGHT_PIN,
    KEYPAD_ANALOG_BUTTON_PIN,
    BEFORE_BUTTON_PIN,
    SELECT_BUTTON_PIN,
    NEXT_BUTTON_PIN,
    INCREASE_BUTTON_PIN,
    DECREASE_BUTTON_PIN,
    RELAY_PIN
>;

// Check for duplicates using template metaprogramming
static constexpr bool has_duplicates = pin_check::CheckDuplicates<
    unsigned,
    PinArray::values[0], PinArray::values[1], PinArray::values[2],
    PinArray::values[3], PinArray::values[4], PinArray::values[5],
    PinArray::values[6], PinArray::values[7], PinArray::values[8],
    PinArray::values[9], PinArray::values[10], PinArray::values[11],
    PinArray::values[12], PinArray::values[13], PinArray::values[14],
    PinArray::values[15]
>::value;

// Trigger compiler error if duplicates exist
static_assert(!has_duplicates, "Duplicate pin values detected");
