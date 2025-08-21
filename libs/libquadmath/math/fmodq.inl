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
    #include "fabsq.inl"
    #include "fmaq.inl"
    #include "copysignq.inl"
#endif

/** \brief 128-bit modulus: remainder of x/y, with same sign as x and |r|<|y|.
 *
 * \param x __float128 Numerator.
 * \param y __float128 Denominator.
 * \return __float128 Remainder.
 */
static inline __float128 fmodq(__float128 x, __float128 y)
{
    // 1) Special cases
    if (isnanq(x) || isnanq(y))     return NANq;
    if (isinfq(x)  || y == 0.0q)     return NANq;  // fmod(inf, y)  or fmod(x,0) → NaN

    // 2) Compute truncated quotient n = trunc(x/y)
    __float128 q = x / y;
    int64_t   n = (int64_t)q;   // C++ cast truncates toward zero

    // 3) Compute remainder r = x – n*y in one rounding via fused-multiply-add
    __float128 r = fmaq(-y, (__float128)n, x);

    // 4) Correct any off-by-one due to rounding
    //    Guarantee |r| < |y|    and    sign(r) == sign(x)
    if (r > 0.0q) {
        if (r >= fabsq(y))      // r == +y can happen if q was rounded down
            r -= copysignq(y, x);
    }
    else if (r < 0.0q) {
        if (r <= -fabsq(y))     // r == -y can happen if q was rounded up
            r += copysignq(y, x);
    }
    // if r==0, it already has the sign of x via fmaq and copysignq in the above

    return r;
}
