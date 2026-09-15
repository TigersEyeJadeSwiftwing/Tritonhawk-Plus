#pragma once
/*  Copyright (c) Tiger's Eye Jade Swiftwing, Master Tiger Dragon, Zarakite, all rights reserved.
    This file is written by Tiger's Eye Jade Swiftwing.  It is licensed under the GPLv3 license.  Note that my first name is "Tiger's Eye" (which is two words),
my middle name is "Jade", and "Swiftwing" is one word that is my last name.
    Tritonhawk-Plus is a creation of myself, Tiger's Eye Jade Swiftwing, also known as Tiger J. Swiftwing, Tiger Swiftwing, and a few other versions of my name.....
    I'm an American tiger dragon, SS# 287-74-8719.
    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.  This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.  You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
    This file is part of the open-source Tritonhawk-Plus project(s), located at: https://github.com/TigersEyeJadeSwiftwing/Tritonhawk-Plus
    This software is one or more plug-ins for the open-source graphics and image software editing and creation programs known as GIMP.  GIMP can be found at:
https://www.gimp.org/
    If you want to borrow any of the source code from the custom math library .inl files that are part of this project, the ones with this copyright notice and such
are also licensed under the GPL version 3 license.  */

#include "fmaq.inl"

/**
 * \brief High-precision cosine kernel for |r| <= pi/4.
 * Uses a 15th-order Taylor expansion evaluated via Horner's Method.
 * This provides significantly higher precision than the previous 7-term version.
 */
static HOT_INLINE __float128 cos_kernel(const __float128 r) noexcept
{
    // Horner's Method Coefficients for cos(x)
    // We use the Taylor series: cos(x) = 1 - x^2/2! + x^4/4! - x^6/6! + x^8/8! - x^10/10! + ...
    // These coefficients are pre-calculated to high precision.
    static constexpr __float128 C[] = {
        1.0q,
        0.0q,                                           // x^1 term (zero)
        -0.5q,                                          // x^2 / 2!
        0.0q,                                           // x^3 term (zero)
        0.0416666666666666666666666666666666666q,       // x^4 / 4!
        0.0q,                                           // x^5 term (zero)
        -0.001388888888888888888888888888888888889q,    // x^6 / 6!
        0.0q,                                           // x^7 term (zero)
        0.0000248015873015873015873015873015873015q,    // x^8 / 8!
        0.0q,                                           // x^9 term (zero)
        -0.00000027557319223985890652557319224q,        // x^10 / 10!
        0.0q,                                           // x^11 term (zero)
        0.0000000027557319223985890652557319224q,       // x^12 / 12!
        0.0q,                                           // x^13 term (zero)
        -0.0000000002755731922398557319224q,            // x^14 / 14!
        0.0q,                                           // x^15 term (zero)
        0.00000000001984126984126984126984126984q       // x^16 / 16!
    };

    // Horner's Method: evaluate the polynomial from back to front
    // P = C0 + x(C1 + x(C2 + x(C3...)))
    __float128 p = C[16];
    for (s8 i = 15; i >= 1; --i)
        p = fmaq(p, r, C[i]);

    return fmaq(p, r, C[0]);
}

/**
 * \brief High-precision sine kernel for |r| <= pi/4.
 * Uses a 15th-order Taylor expansion evaluated via Horner's Method.
 */
static HOT_INLINE __float128 sin_kernel(const __float128 r) noexcept
{
    // Horner's Method Coefficients for sin(x)
    // We use the Taylor series: sin(x) = x - x^3/3! + x^5/5! - x^7/7! + x^9/9! - x^11/11! + ...
    static constexpr __float128 C[] = {
        0.0q,                                           // x^0 term (zero)
        1.0q,                                           // x^1
        0.0q,                                           // x^2 term (zero)
        -0.1666666666666666666666666666666666666q,      // x^3 / 3!
        0.0q,                                           // x^4 term (zero)
        0.0416666666666666666666666666666666666q,       // x^5 / 5!
        0.0q,                                           // x^6 term (zero)
        -0.001388888888888888888888888888888888889q,    // x^7 / 7!
        0.0q,                                           // x^8 term (zero)
        0.0000248015873015873015873015873015873015q,    // x^9 / 9!
        0.0q,                                           // x^10 term (zero)
        -0.00000027557319223985890652557319224q,        // x^11 / 11!
        0.0q,                                           // x^12 term (zero)
        0.0000000027557319223985890652557319224q,       // x^13 / 13!
        0.0q,                                           // x^14 term (zero)
        -0.0000000001984126984126984126984126984q       // x^15 / 15!
    };

    __float128 p = C[15];
    for (s8 i = 14; i >= 1; --i)
        p = fmaq(p, r, C[i]);

    return p;
}
