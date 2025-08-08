#pragma once

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #include <cmath>
    #include <cstdint>
    #include <cfenv>
    #include <type_traits>
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

// expq() :  high-precision exponential

static inline __attribute__((always_inline, hot))
__float128 expq(__float128 x)
{
    inline static constexpr __float128 COEFF_0 = 1.q;
    // inline static constexpr __float128 COEFF_1 = 1.q; // r^1 / 1!
    inline static constexpr __float128 COEFF_2 = 0.5q; // r^2 / 2!
    inline static constexpr __float128 COEFF_3 = 1.0q / 6.0q; // r^3 / 3!
    inline static constexpr __float128 COEFF_4 = 1.0q / 24.0q; // r^4 / 4!
    inline static constexpr __float128 COEFF_5 = 1.0q / 120.0q; // r^5 / 5!
    inline static constexpr __float128 COEFF_6 = 1.0q / 720.0q; // r^6 / 6!

    __float128 kf = x * M_LOG2Eq; // M_LOG2Eq = log_2 e
    long n = (long) std::nearbyint(kf);
    __float128 r = x - __float128(n) * M_LN2q; // M_LN2q = log_e 2
    __float128 r2 = r * r,
               r4 = r2 * r2;
    __float128 Q = COEFF_0
                   + r
                   + COEFF_2 * r2
                   + COEFF_3 * r2 * r
                   + COEFF_4 * r4
                   + COEFF_5 * r4 * r
                   + COEFF_6 * r4 * r2; // Additional term for better accuracy

    return std::ldexp(Q, n);
}
