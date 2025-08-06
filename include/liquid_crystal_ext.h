#pragma once

#include <Arduino.h>   // For byte type
#include <LiquidCrystal.h>   // Include the LiquidCrystal library for LCD control
#include "log.h"

class PolishLCD : public LiquidCrystal {
public:
    // Inherit all LiquidCrystal constructors
    using LiquidCrystal::LiquidCrystal;

    /// Call instead of begin(), to init LCD and load Polish CGRAM chars
    void beginPolish(uint8_t cols, uint8_t rows) {
        LiquidCrystal::begin(cols, rows);
        loadPolishChars();
    }

    /// Print a UTF‑8 encoded C‑string, mapping Polish letters to custom slots
    virtual size_t print(const char *str) {
        size_t count = 0;
        while (*str) {
            uint8_t c = (uint8_t)*str;
            if (c == 0xC4) {
                // two‑byte sequences starting with C4
                str++;
                switch ((uint8_t)*str) {
                    case 0x85: write(byte(0)); break; // ą
                    case 0x87: write(byte(1)); break; // ć
                    case 0x99: write(byte(2)); break; // ę
                    case 0x84: write(byte(4)); break; // ń
                    default:
                        write('?');
                        LOG_INFO("Unknown Polish character: %02X %02X", (int)c, (int)*str);
                        break;
                }
            }
            else if (c == 0xC3) {
                // ó = C3 B3
                str++;
                if ((uint8_t)*str == 0xB3) write(byte(5));
                else {
                    LOG_INFO("Unknown Polish character: %02X %02X", (int)c, (int)*str);
                    write('?');
                }
            }
            else if (c == 0xC5) {
                // two‑byte sequences starting with C5
                str++;
                switch ((uint8_t)*str) {
                    case 0x9B: write(byte(6)); break; // ś
                    case 0xBC: write(byte(7)); break; // ż
                    case 0x82: write(byte(3)); break; // ł
                    default:
                        write('?');
                        LOG_INFO("Unknown Polish character: %02X %02X", (int)c, (int)*str);
                        break;
                }
            }
            else {
                write(c);
            }
            str++;
            count++;
        }
        return count;
    }

    inline size_t print(const float f) {
        return LiquidCrystal::print(f);
    }

    inline size_t print(const size_t s) {
        return LiquidCrystal::print(s);
    }

private:
    static constexpr const uint8_t _polishChars[8][8] = {
        // ą
        {0b00000, 0b00000, 0b01110, 0b00001, 0b01111, 0b10001, 0b01110, 0b00001},
        // ć
        {0b00010, 0b00100, 0b01110, 0b10000, 0b10000, 0b10000, 0b01110, 0b00000},
        // ę
        {0b00000, 0b01110, 0b10001, 0b11111, 0b10000, 0b01111, 0b00010, 0b00100},
        // ł
        {0b01100, 0b00100, 0b00100, 0b00110, 0b01100, 0b00100, 0b00100, 0b00000},
        // ń
        {0b00100, 0b00010, 0b00000, 0b10110, 0b11001, 0b10001, 0b10001, 0b00000},
        // ó
        {0b00010, 0b00100, 0b01110, 0b10001, 0b10001, 0b10001, 0b01110, 0b00000},
        // ś
        {0b00010, 0b00100, 0b01111, 0b10000, 0b01110, 0b00001, 0b11110, 0b00000},
        // ż
        {0b00100, 0b00000, 0b11111, 0b00010, 0b00100, 0b01000, 0b11111, 0b00000}
    };

    void loadPolishChars() {
        for (uint8_t i = 0; i < 8; i++) {
            createChar(i, const_cast<uint8_t*>(_polishChars[i]));
        }
    }
};
