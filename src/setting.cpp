#include "setting.h"
#include "log.h"
#include <stdlib.h>
#include "type_traits_arduino.h"

template<typename T, size_t kNameMaxLen>
Setting<T, kNameMaxLen>::Setting(const char* name,
                                 GetValueFuncType get_value_func,
                                 SetValueFuncType set_value_func,
                                 T step):
      get_value_func_{get_value_func},
      set_value_func_{set_value_func},
      step_{step} {
    static_assert(kNameMaxLen > 0, "kNameMaxLen must be greater than 0");
    if (strlen(name) >= kNameMaxLen) {
        LOG_ERROR("Setting name exceeds maximum length");
        exit(EXIT_FAILURE); // Exit if name exceeds maximum length
    }
    strncpy(name_, name, kNameMaxLen);
    name_[kNameMaxLen - 1] = '\0'; // Ensure null-termination
}

template<typename T, size_t kNameMaxLen>
void Setting<T, kNameMaxLen>::increase() {
  value_ += step_;
}

template<typename T, size_t kNameMaxLen>
void Setting<T, kNameMaxLen>::decrease() {
  value_ -= step_;
}

template<typename T, size_t kNameMaxLen>
const char* Setting<T, kNameMaxLen>::getName() const {
  return name_;
}

template<typename T, size_t kNameMaxLen>
bool Setting<T, kNameMaxLen>::save() const {
  set_value_func_(savedValue_);
  savedValue_ = value_;
  return true;
}

template<typename T, size_t kNameMaxLen>
void Setting<T, kNameMaxLen>::discard() {
  value_ = savedValue_;
}

template<typename T, size_t kNameMaxLen>
void Setting<T, kNameMaxLen>::getDescription(char* buffer, size_t buffer_size) {
  if constexpr (gpt::is_integral_v<T>) {  // constexpr only necessary on first statement
    snprintf(buffer, buffer_size, "%s: %d", name_, value_);
  } else if constexpr (gpt::is_floating_point_v<T>) {  // automatically constexpr
      snprintf(buffer, buffer_size, "%s: %f", name_, value_);
  } else {
      snprintf(buffer, buffer_size, "Unsupported type");
    }
}
