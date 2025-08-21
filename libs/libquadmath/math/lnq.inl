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
    #include "frexpq.inl"
#endif

/** \brief Compute the natural logarithm of a binary128 value.
 *
 * \param x __float128 Input value (must be > 0).
 * \return __float128 Result of ln(x).
 */
static HOT_INLINE __float128 lnq(__float128 x)
{
    // 1) Extract exponent e and mantissa m in [0.5,1)
    s64 e = 0;
    __float128 m = frexpq(x, &e);  // x = m * 2^e, m in [0.5,1)

    if (x == 0) return -INFINITYq;
    if (x < 0) return NANq;
    if (isinfq(x)) return INFINITYq;
    if (isnanq(x)) return NANq;

    // 2) Shift m into [sqrt(1/2), sqrt(2)] to improve polynomial accuracy
    if (m < M_SQRT1_2q) // M_SQRT1_2q = sqrt(1/2)
    {
        m *= 2.q;
        e -= 1;
    }

    // 3) Now m in [sqrt(1/2), sqrt(2)]. Let z = (m-1)/(m+1),
    //    then ln(m) = 2 * [ z + z^3/3 + z^5/5 + ... ]  (arctanh series)
    __float128 z = (m - 1.q) / (m + 1.q);
    __float128 z2 = z * z,
               z4 = z2 * z2,
               z6 = z4 * z2; // z^6 for higher accuracy

    // 4) A small odd polynomial P(z) ≈ arctanh(z) on |z| < 0.17
    // We do only a few terms; you can raise order for more accuracy.
    __float128 P = z
                   + (z * z2) / 3.q
                   + (z * z4) / 5.q
                   + (z * z6) / 7.q
                   + (z * z6 * z2) / 9.q; // Additional term for improved accuracy

    return 2.q * P + __float128(e) * M_LN2q; // M_LN2q = ln(2) as __float128
}
