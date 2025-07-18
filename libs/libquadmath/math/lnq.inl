#pragma once

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #include <cmath>
    #include <cstdint>
    #include <cfenv>
    #include <type_traits>
#endif

/*
Copyright (c) Tiger's Eye Jade Swiftwing, all rights reserved.

This file is written by Tiger's Eye Jade Swiftwing.  It is licensed under the GPLv3 license.
Note that my first name is "Tiger's Eye" (which is two words), my middle name is "Jade", and "Swiftwing" is one word that is my last name.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

static inline __attribute__((always_inline, hot))
__float128 lnq(__float128 x)
{
    // 1) Extract exponent e and mantissa m in [0.5,1)
    int e = 0;
    __float128 m = std::frexp(x, &e);  // x = m * 2^e, m in [0.5,1)

    // 2) Shift m into [sqrt(1/2), sqrt(2)] to improve polynomial accuracy
    if (m < __float128(0.70710678118654752440084436210485q)) // sqrt(1/2)
    {
        m *= 2;
        e -= 1;
    }

    // 3) Now m in [sqrt(1/2), sqrt(2)]. Let z = (m-1)/(m+1),
    //    then ln(m) = 2 * [ z + z^3/3 + z^5/5 + ... ]  (arctanh series)
    __float128 z = (m - 1) / (m + 1);
    __float128 z2 = z*z, z4 = z2*z2;

    // A small odd polynomial P(z) ≈ arctanh(z) on |z| < 0.17
    // We do only a few terms; you can raise order for more accuracy.
    __float128 P = z
            + (z*z2)/3.q
            + (z*z4)/5.q
            + (z*z4*z2)/7.q;

    // static const __float128 ln2 = __float128(0.693147180559945309417232121458176568q);
    return __float128(2.q * P + __float128(e) * 0.693147180559945309417232121458176568q);
}
