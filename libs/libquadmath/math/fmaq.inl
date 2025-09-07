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

/**
 * \brief Computes a*b + c with one rounding error, using a robust TwoProd and TwoSum algorithm.
 *
 * \param a First multiplicand.
 * \param b Second multiplicand.
 * \param c Addend.
 * \return f128 The result of the fused multiply-add operation.
 */
static HOT_INLINE f128 fmaq(const f128 a, const f128 b, const f128 c) noexcept
{
    if (invalidq(a) || invalidq(b) || invalidq(c)) return NANq;

    // --- TwoProd(a, b) ---
    static constexpr f128 SPLIT = 0x1.0p57q + 1.q;
    f128 ap = a * SPLIT, ah = ap - (ap - a), al = a - ah;
    f128 bp = b * SPLIT, bh = bp - (bp - b), bl = b - bh;
    f128 p1 = a * b;
    f128 p2 = ((ah*bh - p1) + ah*bl + al*bh) + al*bl;

    // --- TwoSum(p1, c) ---
    f128 s     = p1 + c;
    f128 err   = (p1 - s) + c;

    return s + (p2 + err);
}
