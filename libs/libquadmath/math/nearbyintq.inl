#pragma once

/*
    Copyright (c) Tiger's Eye Jade Swiftwing, all rights reserved.
    This file is written by Tiger's Eye Jade Swiftwing.  It is licensed under the
GPLv3 license.  Note that my first name is "Tiger's Eye" (which is two words), my
middle name is "Jade", and "Swiftwing" is one word that is my last name.
    This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.  This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.  You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #include "isnanq.inl"
    #include "isinfq.inl"
    #include "fabsq.inl"
    #include "floorq.inl"
    #include "fmodq.inl"
    #include "roundq.inl"
    #include "copysignq.inl"
#endif

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
/** \brief Rounds a __float128 value to the nearest integer, using the current rounding mode.
 *
 * \param x __float128 The input value to round.
 * \return __float128 The nearest integer value as a __float128.
 */
/*
static HOT_INLINE __float128 nearbyintq(__float128 x)
{
    // Handle special cases
    if (isnanq(x) || isinfq(x))
        return x; // Preserve NaN and ±Inf

    // Handle zero
    if (x == 0.0q)
        return 0.0q; // The nearest integer to zero is zero

    // Get the integer part and the fractional part
    __float128 intPart = static_cast<__float128>(static_cast<int64_t>(x)); // Truncate to integer
    __float128 fracPart = x - intPart; // Get the fractional part

    // Handle rounding
    if (fracPart > 0.5q) {
        return intPart + 1.0q; // Round up
    } else if (fracPart < 0.5q) {
        return intPart; // Round down
    } else {
        // If exactly halfway, round to the nearest even integer
        if (static_cast<int64_t>(intPart) % 2 == 0) {
            return intPart; // Even case
        } else {
            return intPart + 1.0q; // Odd case, round up
        }
    }
}
*/

/** \brief Rounds a __float128 value to the nearest integer, using the current rounding mode.
 *
 * \param x __float128 The input value to round.
 * \return s64 The nearest integer value as a signed 64-bit integer.
 */
/*
static HOT_INLINE s64 s64nearbyintq(__float128 x)
{
    // Handle special cases
    if (isnanq(x) || isinfq(x))
        return x; // Preserve NaN and ±Inf

    // Handle zero
    if (x == 0.0q)
        return 0.0q; // The nearest integer to zero is zero

    // Get the integer part and the fractional part
    __float128 intPart = static_cast<__float128>(static_cast<int64_t>(x)); // Truncate to integer
    __float128 fracPart = x - intPart; // Get the fractional part

    __float128 result_f128 = 0.q;

    // Handle rounding
    if (fracPart > 0.5q) {
        result_f128 = intPart + 1.0q; // Round up
    } else if (fracPart < 0.5q) {
        result_f128 = intPart; // Round down
    } else {
        // If exactly halfway, round to the nearest even integer
        if (static_cast<int64_t>(intPart) % 2 == 0) {
            result_f128 = intPart; // Even case
        } else {
            result_f128 = intPart + 1.0q; // Odd case, round up
        }
    }

    if (x < 0.q)
        return -static_cast<s64>(-result_f128 + 0.1q);
    else
        return static_cast<s64>(result_f128 + 0.1q);
}
*/

/** \brief Rounds a __float128 value to the nearest integer, using the current rounding mode.
 *
 * \param x __float128 The input value to round.
 * \return __float128 The nearest integer value as a __float128.
 */
/*
static HOT_INLINE __float128 nearbyintq(__float128 x)
{
    // Handle special cases: NaN and Inf
    if (isnanq(x) || isinfq(x))
        return x;

    // A correct implementation for all numbers, including negative ones.
    __float128 abs_x = fabsq(x);
    __float128 floor_abs_x = floorq(abs_x);
    __float128 frac_part = abs_x - floor_abs_x;

    __float128 result;

    if (frac_part > 0.5q) {
        result = floor_abs_x + 1.0q; // Round away from zero
    } else if (frac_part < 0.5q) {
        result = floor_abs_x; // Round towards zero
    } else {
        // Exactly halfway, round to nearest even.
        if (fmodq(floor_abs_x, 2.0q) == 0.0q) {
            result = floor_abs_x; // Already even
        } else {
            result = floor_abs_x + 1.0q; // Round up to the next even number
        }
    }

    return copysignq(result, x); // Copy the original sign
}
*/

/** \brief Rounds a __float128 value to the nearest integer, using the current rounding mode.
 *
 * \param x __float128 The input value to round.
 * \return __float128 The nearest integer value as a __float128.
 */
static HOT_INLINE __float128 nearbyintq(__float128 x) {
    static constexpr __float128 SHIFT = 0x1.0p112q;  // 2^112

    __float128 ax = x < 0 ? -x : x;
    __float128 y  = (ax + SHIFT) - SHIFT;  // rounds to nearest‐even

    return x < 0 ? -y : y;
}

#else
/** \brief Rounds a long double value to the nearest integer, using the current rounding mode.
 *
 * \param x long double The input value to round.
 * \return long double The nearest integer value as a long double.
 */
static HOT_INLINE long double nearbyintq(long double x)
{
    // Handle special cases
    if (isnanq(x) || isinfq(x))
        return x; // Preserve NaN and ±Inf

    // Handle zero
    if (x == 0.0L)
        return 0.0L; // The nearest integer to zero is zero

    // Get the integer part and the fractional part
    long double intPart = static_cast<long double>(static_cast<int64_t>(x)); // Truncate to integer
    long double fracPart = x - intPart; // Get the fractional part

    // Handle rounding
    if (fracPart > 0.5L) {
        return intPart + 1.0L; // Round up
    } else if (fracPart < 0.5L) {
        return intPart; // Round down
    } else {
        // If exactly halfway, round to the nearest even integer
        if (static_cast<int64_t>(intPart) % 2 == 0) {
            return intPart; // Even case
        } else {
            return intPart + 1.0L; // Odd case, round up
        }
    }
}
#endif
