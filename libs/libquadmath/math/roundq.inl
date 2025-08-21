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
#endif

/** \brief Rounds a value to the nearest whole number.
 *
 * \param x __float128 The input value to round.
 * \return __float128 The nearest whole number the input is rounded to.
 */
static HOT_INLINE __float128 roundq(__float128 x)
{
    if (isnanq(x) || isinfq(x))
        return x;                // Preserve NaN and ±Inf

    uint64_t hi, lo;
    GET_FLT128_WORDS64(hi, lo, x);

    // Extract unbiased exponent j0 = exp - bias
    int j0 = int((hi >> 48) & 0x7FFF) - 0x3FFF;

    // |x| < 0.5 → round toward ±0 to zero
    if (j0 < 0) {
        hi &= 0x8000'0000'0000'0000ULL;         // keep sign
        hi |= 0x3FFF'0000'0000'0000ULL;         // produce ±0.0…5
        lo  = 0;
    }
    // |x| >= 2^112 or NaN/Inf → return x + x to preserve NaN
    else if (j0 > 111) {
        return ((uint16_t)(hi >> 48) == 0x7FFF)
               ? x + x
               : x;
    }
    else {
        // Mask for fractional bits in high and low words
        uint64_t frac_hi = (0x0000'FFFF'FFFF'FFFFULL >> j0);
        uint64_t frac_lo = (j0 < 64)
            ? ~0ULL >> (j0 + 16)
            : 0xFFFF'FFFF'FFFF'FFFFULL;

        // If already integral, exit early
        if (((hi & frac_hi) | (lo & frac_lo)) == 0)
            return x;

        // Add 0.5 ulp at the rounding position
        if (j0 < 64) {
            lo += (1ULL << (63 - j0));
            if (lo < (1ULL << (63 - j0))) {
                hi += 1;  // carry
            }
        } else {
            hi += (1ULL << (112 - j0));
        }

        // Clear fractional bits
        hi &= ~frac_hi;
        lo &= ~frac_lo;
    }

    SET_FLT128_WORDS64(x, hi, lo);
    return x;
}
