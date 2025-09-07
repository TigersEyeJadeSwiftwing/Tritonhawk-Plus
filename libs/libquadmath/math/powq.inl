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

#include "expq.inl"
#include "lnq.inl"
#include "modfq.inl"

/** \brief Calculate x raised to the power of y.
 *
 * \param x f128 The base value.
 * \param y f128 The exponent value.
 * \return f128 The resulting value of x^y.
 *
 * Special-cases:
 *   - If x == 0 and y == 0: returns 1.
 *   - If x == 0 and y > 0: returns 0.
 *   - If x == 0 and y < 0: returns +∞.
 *   - If x < 0 and y is an integer: returns sign(x^y) * exp(y*ln|x|).
 *   - If x < 0 and y not integer: returns NaN.
 *   - If x == 1 or y == 1: returns x.
 *   - If y == 0: returns 1.
 */
static HOT_INLINE f128 powq(const f128 x, const f128 y) noexcept
{
    // Check for NaNs
    if (invalidq(x) || invalidq(y)) return NANq;

    // Fast-path trivial cases
    if ((x == 1.q) || (y == 1.q)) return x;
    if (y == 0.q) return 1.q;

    if (x == 0.q)
    {
        if (y > 0.q) return 0.q;
        if (y < 0.q) return INFINITYq;
    }

    // Negative base: only valid for integer exponents
    if (x < 0.q) {
        f128 intpart;
        f128 frac = modfq(y, &intpart);
        if (frac != 0.q)
        {
            // non-integer exponent of negative → NaN
            return NANq;
        }
        // integer exponent: determine sign
        // odd integer → negative result; even → positive
        int64_t ival = (int64_t)intpart;
        f128 magnitude = expq(y * lnq(-x));
        return ((ival & 1) ? -magnitude : magnitude);
    }

    // General positive-base case: exp(y * ln(x))
    return expq(y * lnq(x));
}
