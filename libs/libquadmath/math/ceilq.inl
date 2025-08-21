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
#endif

/** \brief Gets the nearest whole number that is equal to or greater than the input value.
 *
 * \param x __float128 The input number to round up.
 * \return __float128 The nearest whole number that is always equal to or greater than the input.
 */
static HOT_INLINE __float128 ceilq(__float128 x)
{
    if (isnanq(x) || isinfq(x))
        return x;                // Preserve NaN and ±Inf

    static constexpr __float128 TWO_POW_113_base = 9007199254740992.0q;
    static constexpr __float128 TWO_POW_113 = TWO_POW_113_base * TWO_POW_113_base;

    // For very large |x|, x is already integral
    if (fabsq(x) >= TWO_POW_113)
        return x;

    __float128 y;
    if (x >= 0.q) {
        // Truncate toward zero yields floor for positive x
        y = x + TWO_POW_113; // shift fractional bits out
        y = y - TWO_POW_113;
        // If there was any fractional part, bump up
        return (y == x) ? y : (y + 1.q);
    } else {
        // For negative x, truncation toward zero = ceil
        y = x - TWO_POW_113; // shift fractional bits out
        y = y + TWO_POW_113;
        return y;
    }
}
