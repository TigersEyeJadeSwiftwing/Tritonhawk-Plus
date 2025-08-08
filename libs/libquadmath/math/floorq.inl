#pragma once

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #include "fabsq.inl"
#endif

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

static inline __attribute__((always_inline, hot))
__float128 floorq(__float128 x) {
    if (__builtin_isnanq(x) || __builtin_isinfq(x))
        return x;                // Preserve NaN and ±Inf

    inline static constexpr __float128 TWO_POW_113 = ((__float128)1) * ( (__float128)1 << 113 );

    // For very large |x|, x is already integral
    if (fabsq(x) >= TWO_POW_113)
        return x;

    __float128 y;
    if (x >= 0.q) {
        y = x + TWO_POW_113; // shift fractional bits out
        y = y - TWO_POW_113;
        // y is truncated toward zero → same as floor for x ≥ 0
        return y;
    } else {
        // For negatives, truncation gives ceil. To get floor, subtract 1.
        y = x - TWO_POW_113; // shift fractional bits out
        y = y + TWO_POW_113;
        return (y == x) ? y : (y - 1.q);
    }
}
