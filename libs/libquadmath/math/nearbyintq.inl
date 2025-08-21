#pragma once
/*  Copyright (c) Tiger's Eye Jade Swiftwing, all rights reserved.
    This file is written by Tiger's Eye Jade Swiftwing.  It is licensed under the
GPLv3 license.  Note that my first name is "Tiger's Eye" (which is two words), my
middle name is "Jade", and "Swiftwing" is one word that is my last name.
    Tritonhawk-Plus is a creation of Tiger's Eye Jade Swiftwing, also known as
Tiger J. Swiftwing, Tiger Swiftwing, and a few other versions of my name...
    This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.  This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.  You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>.
    This file is part of the open-source Tritonhawk-Plus project(s), located at:
https://github.com/TigersEyeJadeSwiftwing/Tritonhawk-Plus
    This software is one or more plug-ins for the open-source graphics and image
software editing and creation programs known as GIMP.  GIMP can be found at:
https://www.gimp.org/
    If you want to borrow any of the source code from the custom math library .inl files
that are part of this project, the ones with this copyright notice and such are also
licensed under the GPL version 3 license. */

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
