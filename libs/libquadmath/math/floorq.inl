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

#include "fabsq.inl"

/** \brief Returns the greatest integer (as a float128) that is
 *         less than or equal to the argument.
 *
 *  \param x  The value to round down (float128).
 *  \return   The floor of x.
 */
static HOT_INLINE f128 floorq(const f128 x) noexcept
{
    if (invalidq(x)) return x;               // Preserve NaN and ±Inf

    // 2^113 – the smallest power of two whose ulp is > 1 for binary128
    static constexpr f128 C = 0x1p113Q;       // exact 2^113 literal

    // Numbers whose magnitude is already ≥ C are integral already
    if (fabsq(x) >= C) return x;

    f128 y;
    if (x >= 0.0Q) {
        // Positive: truncate toward zero (same as floor)
        y = x + C;            // shift fractional bits far enough out
        y = y - C;            // drop the constant, leaving integer part
        // If the rounding pushed us *up* (y > x), we overshot by 1 ulp → subtract 1
        return (y > x) ? (y - 1.0Q) : y;
    } else {
        // Negative: truncation toward zero yields ceil, so we must go one step lower
        y = x - C;            // shift fractional bits out (negative direction)
        y = y + C;
        // If rounding gave us a value that is *greater* (less negative) than x,
        // we lost the fractional part and must subtract 1 to get the true floor.
        return (y > x) ? (y - 1.0Q) : y;
    }
}
