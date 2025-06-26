#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>

enum class LogSource {
    UsbSerial,
    Serial1,
    UsbAndSerial1,
    NumberOfSerials
};

inline void initLog() {
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);
    Log.noticeln("Log initialized");
}

inline bool changeLogLevel(const size_t &log_level) {
    if (log_level <= LOG_LEVEL_VERBOSE) {
        Log.setLevel(log_level);
        return true;
    }
    return false;
}

inline void chengeLogSource(const LogSource &log_source) {
    switch (log_source) {
        case LogSource::UsbSerial:
            Log.begin(LOG_LEVEL_NOTICE, &Serial);
            break;
        // case LogSource::Serial1:
        //     Log.begin(LOG_LEVEL_NOTICE, &Serial1);
        //     break;
        // TODO: Implement this function
        // case LogSource::UsbAndSerial1:
        //     break;
        default:
            Log.begin(LOG_LEVEL_NOTICE, &Serial);
            break;
    }
}

inline unsigned long log_number = {};

// logCount [timestamp[ms]] filename:line_number - message
#define LOG_PREFIX "%l [%l] %s:%d- "
#define LOG_MS millis()
#ifndef __FILENAME__
#define __FILENAME__ (                                      \
    __builtin_strrchr(__FILE__, '/')                         \
      ? __builtin_strrchr(__FILE__, '/') + 1                 \
      : (                                                    \
          __builtin_strrchr(__FILE__, '\\')                  \
            ? __builtin_strrchr(__FILE__, '\\') + 1          \
            : __FILE__                                       \
        )                                                    \
)
#endif

#ifdef ENABLE_LOGGING
#define LOG_ERROR(MSG, ...) Log.errorln(LOG_PREFIX MSG, log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_WARNING(MSG, ...) Log.warningln(LOG_PREFIX MSG, log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_NOTICE(MSG, ...) Log.noticeln(LOG_PREFIX MSG, log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(MSG, ...) Log.infoln(LOG_PREFIX MSG, log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(MSG, ...) Log.traceln(LOG_PREFIX MSG, log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_VERBOSE(MSG, ...) Log.verboseln(LOG_PREFIX MSG, log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__)
#else
#define LOG_ERROR(MSG, ...)
#define LOG_WARNING(MSG, ...)
#define LOG_NOTICE(MSG, ...)
#define LOG_INFO(MSG, ...)
#define LOG_DEBUG(MSG, ...)
#define LOG_VERBOSE(MSG, ...)
#endif
