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
#endif

/** \brief 128-bit float truncate function.
 *
 * \param x __float128 Input value.
 * \return __float128 Output value, truncated.
 */
static HOT_INLINE __float128 truncq(__float128 x)
{
    if (isnanq(x) || isinfq(x))
        return x;                // Preserve NaN and ±Inf

    uint64_t hi, lo;
    GET_FLT128_WORDS64(hi, lo, x);

    // Extract unbiased exponent: exp = E − bias
    int exp = int((hi >> 48) & 0x7FFF) - 0x3FFF;

    if (exp < 0) {
        // |x| < 1.0 → result is ±0
        hi &= 0x8000000000000000ULL;
        lo  = 0;
    }
    else if (exp < 48) {
        // 1 ≤ |x| < 2^48 → clear fractional bits in high word
        uint64_t mask_hi = UINT64_C(0x0000FFFFFFFFFFFF) >> exp;
        hi &= ~mask_hi;
        lo  = 0;
    }
    else if (exp < 112) {
        // 2^48 ≤ |x| < 2^112 → clear fractional bits in low word
        uint64_t mask_lo = ~UINT64_C(0) >> (exp - 48);
        lo &= ~mask_lo;
    }
    else {
        // |x| ≥ 2^112 or NaN/Inf
        uint16_t e = uint16_t((hi >> 48) & 0x7FFF);
        if (e == 0x7FFF)     // NaN or ±Inf
            return x + x;   // preserve NaN payload or Inf sign
        // otherwise |x| is already integral
        return x;
    }

    SET_FLT128_WORDS64(x, hi, lo);
    return x;
}
