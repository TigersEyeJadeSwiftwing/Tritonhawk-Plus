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

#include "nearbyintq.inl"
#include "ldexpq.inl"

/** \brief Compute the exponential function of a binary128 value.
 *
 * \param t f128 Exponent.
 * \return f128 Result of e^x.
 */
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
