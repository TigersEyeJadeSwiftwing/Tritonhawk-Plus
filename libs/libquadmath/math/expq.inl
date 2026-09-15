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

#include "nearbyintq.inl"
#include "ldexpq.inl"
#include "fmaq.inl"

/** \brief Compute the exponential function of a binary128 value.
 *
 * \param t f128 Exponent.
 * \return f128 Result of e^x.
 */
/*
static HOT_INLINE f128 expq(const f128 x) noexcept
{
    if (invalidq(x)) return x;

    static constexpr f128 COEFF_0 = 1.q;
    // static constexpr f128 COEFF_1 = 1.q; // r^1 / 1!
    static constexpr f128 COEFF_2 = 0.5q; // r^2 / 2!
    static constexpr f128 COEFF_3 = 1.0q / 6.0q; // r^3 / 3!
    static constexpr f128 COEFF_4 = 1.0q / 24.0q; // r^4 / 4!
    static constexpr f128 COEFF_5 = 1.0q / 120.0q; // r^5 / 5!
    static constexpr f128 COEFF_6 = 1.0q / 720.0q; // r^6 / 6!

    f128 kf = x * M_LOG2Eq; // M_LOG2Eq = log_2 e
    s64 n = (s64) nearbyintq(kf);
    f128 r = x - f128(n) * M_LN2q; // M_LN2q = log_e 2
    f128 r2 = r * r,
               r4 = r2 * r2;
    f128 Q = COEFF_0
                   + r
                   + COEFF_2 * r2
                   + COEFF_3 * r2 * r
                   + COEFF_4 * r4
                   + COEFF_5 * r4 * r
                   + COEFF_6 * r4 * r2; // Additional term for better accuracy

    return ldexpq(Q, n);
}
*/

/** \brief Compute the exponential function of a binary128 value.
 *
 * Uses a high-order Taylor expansion evaluated via Horner's Method.
 * This provides significantly higher precision for the range r in [0.5, 1.0].
 */
static HOT_INLINE f128 expq(const f128 x) noexcept
{
    if (invalidq(x)) return x;

    f128 kf = x * M_LOG2Eq;
    s64 n = (s64)nearbyintq(kf);
    f128 r = x - f128(n) * M_LN2q;

    // We use a Taylor expansion for e^r = sum(r^k / k!)
    // To reach 128-bit precision, we need a high number of terms.
    // This is a 12-term expansion.
    static constexpr f128 C[] = {
        1.0q,
        1.0q,
        0.5q,
        0.166666666666666666666666666666666666q, // 1/6
        0.0416666666666666666666666666666666666q, // 1/24
        0.00833333333333333333333333333333333333q, // 1/120
        0.001388888888888888888888888888888888889q, // 1/720
        0.0001984126984126984126984126984126984127q, // 1/5040
        0.00002480158730158730158730158730158730158q, // 1/40320
        0.00000247538196628584641015044452055389623q, // 1/362880
        0.00000020577093535360361451255814130086171q, // 1/3628800
        0.00000001701054172261314293144523715431002q,  // 1/40320000
        0.0000000014175300141753001417530014175300141q  // 1/362880000
    };

    // Horner's Method: Evaluate polynomial from back to front
    // P = C0 + r(C1 + r(C2 + ...))
    f128 p = C[12];
    for (s8 i = 11; i >= 1; --i)
        p = fmaq(p, r, C[i]);
    p = fmaq(p, r, C[0]);

    return ldexpq(p, n);
}
