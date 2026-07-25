#include <cmath>
#include <limits>

// Round a floating-point value to the nearest multiple
template<typename T>
T roundToMultiple(T value, T multiple) {
    static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");

    if (multiple == 0) {
        return value; // Avoid division by zero
    }

    T divided = value / multiple;
    T rounded = std::round(divided);
    T result = rounded * multiple;

    // Apply epsilon correction for values very close to integers
    // This handles floating-point precision errors
    T epsilon = std::numeric_limits<T>::epsilon() * std::abs(result) * 10;

    if (std::abs(result - std::round(result)) < epsilon) {
        result = std::round(result);
    }

    return result;
}

template<typename T>
T roundToMultiple(T value, T multiple) {
    static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");

    if (multiple == 0) {
        return value;
    }

    T result = std::round(value / multiple) * multiple;

    // Clean up near-integer values
    T nearest_int = std::round(result);
    T tolerance = multiple * std::numeric_limits<T>::epsilon() * 10;

    if (std::abs(result - nearest_int) < tolerance) {
        return nearest_int;
    }

    return result;
}

#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::fixed << std::setprecision(10);

    // Rounding to nearest 10.0
    std::cout << roundToMultiple(9.9999972386, 10.0) << std::endl;  // Output: 10.0
    std::cout << roundToMultiple(14.5, 10.0) << std::endl;          // Output: 10.0
    std::cout << roundToMultiple(15.0, 10.0) << std::endl;          // Output: 20.0

    // Rounding to nearest 0.01
    std::cout << roundToMultiple(1.2349999, 0.01) << std::endl;     // Output: 1.23
    std::cout << roundToMultiple(1.2350001, 0.01) << std::endl;     // Output: 1.24

    // Rounding to nearest 100.0
    std::cout << roundToMultiple(93700.00000283, 100.0) << std::endl; // Output: 93700.0

    return 0;
}

template<typename T>
T roundToMultiple(T value, T multiple) {
    if (multiple == 0) return value;

    // Do calculations in higher precision
    long double hires_value = static_cast<long double>(value);
    long double hires_multiple = static_cast<long double>(multiple);
    long double hires_result = std::round(hires_value / hires_multiple) * hires_multiple;

    return static_cast<T>(hires_result);
}

That's an excellent question about floating-point precision. Here's how to create a robust C++ function for rounding to arbitrary multiples while handling the precision issues you mentioned:

## The Problem

When you do arithmetic like `round(value / multiple) * multiple`, floating-point errors accumulate. For example:
- `round(9.9999972386 / 1.0) * 1.0` might give `9.9999972386` instead of `10.0`
- Division and multiplication introduce rounding errors

## Solution: Use a Small Epsilon for Correction

Here's a robust approach:

```cpp
#include <cmath>
#include <limits>

// Round a floating-point value to the nearest multiple
template<typename T>
T roundToMultiple(T value, T multiple) {
    static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");

    if (multiple == 0) {
        return value; // Avoid division by zero
    }

    T divided = value / multiple;
    T rounded = std::round(divided);
    T result = rounded * multiple;

    // Apply epsilon correction for values very close to integers
    // This handles floating-point precision errors
    T epsilon = std::numeric_limits<T>::epsilon() * std::abs(result) * 10;

    if (std::abs(result - std::round(result)) < epsilon) {
        result = std::round(result);
    }

    return result;
}
```

## Better Solution: Epsilon-Based Rounding

For more control, here's a version that's more explicit:

```cpp
template<typename T>
T roundToMultiple(T value, T multiple) {
    static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");

    if (multiple == 0) {
        return value;
    }

    T result = std::round(value / multiple) * multiple;

    // Clean up near-integer values
    T nearest_int = std::round(result);
    T tolerance = multiple * std::numeric_limits<T>::epsilon() * 10;

    if (std::abs(result - nearest_int) < tolerance) {
        return nearest_int;
    }

    return result;
}
```

## Practical Example Usage

```cpp
#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::fixed << std::setprecision(10);

    // Rounding to nearest 10.0
    std::cout << roundToMultiple(9.9999972386, 10.0) << std::endl;  // Output: 10.0
    std::cout << roundToMultiple(14.5, 10.0) << std::endl;          // Output: 10.0
    std::cout << roundToMultiple(15.0, 10.0) << std::endl;          // Output: 20.0

    // Rounding to nearest 0.01
    std::cout << roundToMultiple(1.2349999, 0.01) << std::endl;     // Output: 1.23
    std::cout << roundToMultiple(1.2350001, 0.01) << std::endl;     // Output: 1.24

    // Rounding to nearest 100.0
    std::cout << roundToMultiple(93700.00000283, 100.0) << std::endl; // Output: 93700.0

    return 0;
}
```

## Key Points

1. **`std::numeric_limits<T>::epsilon()`** — Returns the smallest representable difference from 1.0 for the type. Multiply by 10 (or higher factor) for a tolerance band.

2. **Check against nearest integer** — If the result is very close to a whole number, replace it with that integer (this catches cases like `9.9999972386`).

3. **Adaptive epsilon** — Scale epsilon by the magnitude of the result so it works for very large and very small numbers.

4. **Template function** — Works with both `float` and `double`.

## Alternative: Use Higher-Precision Intermediate Calculations

If you need even more precision, you can do intermediate calculations in `long double`:

```cpp
template<typename T>
T roundToMultiple(T value, T multiple) {
    if (multiple == 0) return value;

    // Do calculations in higher precision
    long double hires_value = static_cast<long double>(value);
    long double hires_multiple = static_cast<long double>(multiple);
    long double hires_result = std::round(hires_value / hires_multiple) * hires_multiple;

    return static_cast<T>(hires_result);
}
```

This approach performs the rounding calculation in higher precision (`long double`) and then converts back to your target type, which naturally eliminates many precision issues.

Choose the approach that fits your precision requirements and performance constraints!
