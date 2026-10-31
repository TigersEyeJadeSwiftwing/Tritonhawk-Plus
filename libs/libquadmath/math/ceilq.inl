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

/** \brief Returns the smallest integer (as a float128) that is
 *         greater than or equal to the argument.
 *
 *  \param x  The value to round up (float128).
 *  \return   The ceiling of x.
 */
static HOT_INLINE f128 ceilq(const f128 x) noexcept
{
    if (invalidq(x)) return x;               // Preserve NaN and ±Inf

    // 2^113 – the smallest power of two whose ulp is > 1 for binary128
    static constexpr f128 C = 0x1p113Q;       // exact 2^113 literal

    // Any |x| ≥ C is already integral (spacing is 2, so no fraction can exist)
    if (fabsq(x) >= C) return x;

    f128 y;
    if (x >= 0.0Q) {
        // Positive: round‑trip with +C, then add 1 if we lost any fraction
        y = x + C;
        y = y - C;
        return (y < x) ? (y + 1.0Q) : y;      // y < x ⇔ fractional part existed
    } else {
        // Negative: round‑trip with –C, then add 1 if we overshot downward
        y = x - C;
        y = y + C;
        return (y < x) ? (y + 1.0Q) : y;      // e.g. -1.9 → y=-2 → add 1 → -1
    }
}
