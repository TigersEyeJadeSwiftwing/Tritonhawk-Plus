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

// expq:  high-precision exponential

static inline __attribute__((always_inline, hot))
__float128 expq(__float128 x)
{
    __float128 kf = x * 1.442695040888963407359924681001892137q;
    long n = (long) std::nearbyint(kf);
    __float128 r = x - __float128(n) * 0.693147180559945309417232121458176568q;
    __float128 r2 = r*r;
    __float128 Q = __float128(1)
            + r
            + r2*(__float128(0.5q))
            + r2*r*(__float128(1.0q/6.0q))
            + r2*r2*(__float128(1.0q/24.0q))
            + r2*r2*r*(__float128(1.0q/120.0q));

    return std::ldexp(Q, n);
}
