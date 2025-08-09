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

static inline __attribute__((always_inline, hot))
__float128 scalbnq(__float128 x, int n) {
    uint64_t hi, lo;
    GET_FLT128_WORDS64(hi, lo, x);

    // Extract sign, exponent, and mantissa mask
    uint64_t sign = hi & UINT64_C(0x8000000000000000);
    int      exp  = int((hi >> 48) & 0x7FFF);
    const int  BIAS = 0x3FFF;

    // Handle zero, NaN, Inf
    if (exp == 0) {
        // Zero or subnormal: scale x by 2^114 to normalize, adjust exponent
        if ((lo | (hi & UINT64_C(0x7FFFFFFFFFFF'FFFF))) == 0)
            return x;  // zero
        constexpr __float128 two114 = (__float128)0x1.0p114Q;
        x *= two114;
        GET_FLT128_WORDS64(hi, lo, x);
        exp = int((hi >> 48) & 0x7FFF) - 114;
    }
    else if (exp == 0x7FFF) {
        // NaN or Inf
        return x + x;
    }

    // Compute new exponent and check overflow/underflow bounds
    int newe = exp + n;
    if (newe > 0x7FFE) {
        // overflow
        constexpr __float128 huge = (__float128)1.0e+4900Q;
        return copysignq(huge, x);
    }
    if (newe <= 0) {
        // underflow to subnormal or zero
        if (newe <= -114) {
            constexpr __float128 tiny = (__float128)1.0e-4900Q;
            return copysignq(tiny, x);
        }
        // extract subnormal: shift into mantissa
        hi = sign | 0;        // zero out exponent, mantissa
        lo = 0;
        SET_FLT128_WORDS64(x, hi, lo);
        constexpr __float128 twom114 = (__float128)0x1.0p-114Q;
        return x * twom114 * __builtin_powi((__float128)2.0Q, newe + 114);
    }

    // Normal range: just reassemble sign | new exponent | mantissa
    hi = sign | (uint64_t(newe) << 48) | (hi & UINT64_C(0x0000FFFFFFFF'FFFF));
    SET_FLT128_WORDS64(x, hi, lo);
    return x;
}
