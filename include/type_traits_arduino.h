#pragma once

namespace gpt {
// Minimal integral_constant
template<typename T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;
    constexpr operator value_type() const noexcept { return value; }
};

// true_type / false_type
using true_type  = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

// Base template for false value
template<typename T>
struct is_integral : false_type {};

// Specializations for integral types
template<> struct is_integral<bool>               : true_type {};
template<> struct is_integral<char>               : true_type {};
template<> struct is_integral<signed char>        : true_type {};
template<> struct is_integral<unsigned char>      : true_type {};
template<> struct is_integral<short>              : true_type {};
template<> struct is_integral<unsigned short>     : true_type {};
template<> struct is_integral<int>                : true_type {};
template<> struct is_integral<unsigned int>       : true_type {};
template<> struct is_integral<long>               : true_type {};
template<> struct is_integral<unsigned long>      : true_type {};
template<> struct is_integral<long long>          : true_type {};
template<> struct is_integral<unsigned long long> : true_type {};

template<typename T>
constexpr bool is_integral_v = is_integral<T>::value;

// -----------------------------

template<typename T>
struct is_floating_point : false_type {};

// Specializations for floating point types
template<> struct is_floating_point<float>       : true_type {};
template<> struct is_floating_point<double>      : true_type {};
template<> struct is_floating_point<long double> : true_type {};

template<typename T>
constexpr bool is_floating_point_v = is_floating_point<T>::value;
} // namespace gpt
