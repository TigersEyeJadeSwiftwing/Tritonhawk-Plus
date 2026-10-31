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
 * \brief High-precision sine kernel for |r| <= pi/4.
 * Evaluated as r * P(r^2) using Horner's Method with accurate coefficients.
 */
static HOT_INLINE __float128 sin_kernel(const __float128 r) noexcept
{
    // Coefficients for sin(x) / x in terms of r^2 (34+ decimal digits precision)
    static constexpr __float128 C[] = {
        1.0q,                                                 // x^1 / 1!
        -0.1666666666666666666666666666666667q,            // x^3 / 3!
        0.0083333333333333333333333333333333q,             // x^5 / 5!
        -0.0001984126984126984126984126984127q,          // x^7 / 7!
        0.0000027557319223985890652557319224q,          // x^9 / 9!
        -0.0000000250521083854417187750393362q,         // x^11 / 11!
        0.0000000001605904383682161459939396q,         // x^13 / 13!
        -0.0000000000007647163731819816475901q          // x^15 / 15!
    };

    __float128 r2 = r * r;

    __float128 p = C[7];
    for (int i = 6; i >= 0; --i) {
        p = fmaq(p, r2, C[i]);
    }

    return r * p;
}

/**
 * \brief High-precision cosine kernel for |r| <= pi/4.
 * Evaluated as P(r^2) using Horner's Method with accurate coefficients.
 */
static HOT_INLINE __float128 cos_kernel(const __float128 r) noexcept
{
    // Coefficients for cos(x) in terms of r^2 (34+ decimal digits precision)
    static constexpr __float128 C[] = {
        1.0q,                                                 // x^0 / 0!
        -0.5q,                                                // x^2 / 2!
        0.0416666666666666666666666666666667q,             // x^4 / 4!
        -0.0013888888888888888888888888888889q,          // x^6 / 6!
        0.0000248015873015873015873015873016q,          // x^8 / 8!
        -0.0000002755731922398589065255731922q,         // x^10 / 10!
        0.0000000020876756987868098979210116q,         // x^12 / 12!
        -0.0000000000114706868620479406059002q,        // x^14 / 14!
        0.0000000000000477947733246237197088q          // x^16 / 16!
    };

    __float128 r2 = r * r;

    __float128 p = C[8];
    for (int i = 7; i >= 0; --i) {
        p = fmaq(p, r2, C[i]);
    }

    return p;
}
