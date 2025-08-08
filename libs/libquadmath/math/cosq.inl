#pragma once

#include <stdint.h>

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
__float128 cosq(__float128 x)
{
    // Precomputed leading 192 bits of 2/π in quad precision (hex)
    inline static constexpr __float128 TWO_OVER_PI_HI[3] = {
        (__float128) 0x28be60db9391054aULL,  // high 64 bits
        (__float128) 0x84a8ff7988eaadbbULL,  // mid 64 bits
        (__float128) 0xfb8a7e8f1e2863f2ULL   // low 64 bits
    };

    // Precomputed π/2 in quad precision (hex)
    inline static constexpr __float128 PI_OVER_2_HI[3] = {
        (__float128) 0xc90fdaa22168c234ULL,
        (__float128) 0xc4c6628b80dc1cd1ULL,
        (__float128) 0x29024e088a67cc74ULL
    };
    inline static constexpr __float128 PI_OVER_2_MI[3] = {
        (__float128) 0xa918fdc8ba30fbc2ULL,
        (__float128) 0x86e220b9f33d1d1eULL,
        (__float128) 0xdfe3c84680b74a13ULL
    };
    inline static constexpr __float128 PI_OVER_2_LO[3] = {
        (__float128) 0x4f29411aea1c3c7ULL,
        (__float128) 0x9e8b6c9d60f3c0fULL,
        (__float128) 0x2f066c59fcf53ceULL
    };

    inline static constexpr __float128 C[] = {
        // Coefficients for P(r^2) = 1 + c1·r² + c2·r⁴ + ... + c7·r¹⁴
        ( (__float128) -0.5L),
        ( (__float128) +0.041666666666666666666666666666666666L),
        ( (__float128) -0.001388888888888888888888888888888889L),
        ( (__float128) +0.000024801587301587301587301587301587L),
        ( (__float128) -0.00000027557319223985890652557319224L),
        ( (__float128) +0.00000000208767569878680989792100903L),
        ( (__float128) -0.00000000001147074559772972635174139L)
    };

    // 1) Handle NaN/Inf
    if (x != x || x == (__float128)INFINITY || x == -(__float128)INFINITY)
        return (__float128)NAN;

    // 2) Payne–Hanek: compute n = nearest integer to x*(2/π)
    //    We do a 192-bit multiply via three 64-bit pieces.
    __float128 z0 = x * TWO_OVER_PI_HI[0];
    __float128 z1 = x * TWO_OVER_PI_HI[1];
    __float128 z2 = x * TWO_OVER_PI_HI[2];
    // sum with correct alignment:
    __float128 zn = z0
                  + (z1 * (__float128)0x1p-64L)
                  + (z2 * (__float128)0x1p-128L);
    // n = integer part of zn (round to nearest even):
    int64_t n = (int64_t) (__builtin_nearbyintl(zn));

    // 3) Reconstruct r = x - n*(π/2) using hi, mid, lo
    __float128 prod_hi = PI_OVER_2_HI[0] * (__float128)n;
    __float128 prod_mi = PI_OVER_2_MI[0] * (__float128)n;
    __float128 prod_lo = PI_OVER_2_LO[0] * (__float128)n;
    __float128 r = x;
    r = r - prod_hi;
    r = r - prod_mi;
    r = r - prod_lo;

    // 4) Reduce r into [0, π/2)
    int quadrant = n & 3;
    bool negate = (quadrant == 1 || quadrant == 2);
    bool swap_xy = (quadrant == 1 || quadrant == 3);
    // For cos: use cos(r) or sin(r) based on quadrant
    // If swap_xy, compute sin from cos polynomial via cos(π/2 - r).
    if (swap_xy) {
        r = M_PI_2q - r;
    }

    // 5) Evaluate minimax polynomial P(r²) with Horner + FMA
    __float128 r2 = r * r;
    __float128 p = C[6];
    for (int i = 5; i >= 0; --i) {
        p = __builtin_fmaf(p, r2, C[i]);
    }
    __float128 y = __builtin_fmaf(p, r2, 1.q);

    // 6) Adjust sign for the quadrant
    return negate ? -y : y;
}
