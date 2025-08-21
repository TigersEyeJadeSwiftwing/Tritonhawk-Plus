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

/** \brief Computes a * 2^exp for __float128 data types.
 *
 * \param a __float128 The base value to scale.
 * \param exp s64 The exponent to which 2 is raised.
 * \return __float128 The result of a * 2^exp.
 */
static HOT_INLINE __float128 ldexpq(__float128 a, s64 exp)
{
    if (isnanq(a) || isinfq(a)) {
        return a; // Preserve NaN and ±Inf
    }

    // Handle the case where exp is zero
    if (exp == 0) {
        return a; // a * 2^0 = a
    }

    // Calculate the scaling factor as __float128
    static constexpr __float128 TWO_POW_64 = 18446744073709551616.0q; // 2^64
    __float128 result;

    if (exp > 0) {
        // For positive exponents
        if (exp >= 64) {
            result = a * (TWO_POW_64 * (1.0q * (1ULL << (exp - 64)))); // Scale by 2^64 and shift
        } else {
            result = a * (1.0q * (1ULL << exp)); // Scale by 2^exp directly
        }
    } else {
        // For negative exponents
        if (exp <= -64) {
            result = a / (TWO_POW_64 * (1.0q * (1ULL << (-exp - 64)))); // Scale down by 2^64
        } else {
            result = a / (1.0q * (1ULL << -exp)); // Scale down by 2^(-exp) directly
        }
    }

    return result;
}
