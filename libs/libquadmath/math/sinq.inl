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
#include "fmaq.inl"

#include "sin_cos_kernel.inl"

/** \brief 128-bit float sine function.
 *
 * \param x f128 Input value.
 * \return f128 Output value, sine of input.
 */
static HOT_INLINE f128 sinq_old(const f128 x) noexcept
{
    if (invalidq(x)) return x;

    // a) Argument reduction: n = round(x * 2/π)
    f128 prod_hi = x * TWO_OVER_PI_HI;
    f128 prod_lo = x * TWO_OVER_PI_LO;
    f128 z       = prod_hi + prod_lo;

    int64_t n = static_cast<int64_t>(nearbyintq(z));

    // b) Reconstruct r = x – n * (π/2) using the 3-term split
    f128 r = ((x - n * PI_OVER_2_HI)
                - n * PI_OVER_2_MI)
                - n * PI_OVER_2_LO;

    // c) Determine base quadrant (handle negative n safely in C++)
    int64_t q = n % 4;
    if (q < 0) q += 4;

    // d) Fold |r| into <= π/4 and adjust quadrant q accordingly
    static constexpr f128 PI_4 = M_PI_4q; // π/4
    static constexpr f128 PI_2 = M_PI_2q; // π/2

    if (r > PI_4) {
        r = PI_2 - r;
        q += 1;
    }
    else if (r < -PI_4) {
        r = -PI_2 - r;
        q -= 1;
    }
    q = (q + 4) % 4; // Normalize q to [0, 3]

    // e) Select kernel and sign based on final q for sine:
    //    q = 0 :  sin(r)
    //    q = 1 :  cos(r)
    //    q = 2 : -sin(r)
    //    q = 3 : -cos(r)
    bool useSin = (q == 0) || (q == 2);
    bool negate = (q >= 2);

    f128 y = useSin ? sin_kernel(r) : cos_kernel(r);
    return negate ? -y : y;
}

/** \brief 128-bit float sine function.
 *
 * \param x f128 Input value.
 * \return f128 Output value, sine of input.
 */
static HOT_INLINE f128 sinq(const f128 x) noexcept
{
    if (invalidq(x)) return x;

    // a) Argument reduction: n = round(x * 2/π)
    f128 prod_hi = x * TWO_OVER_PI_HI;
    f128 prod_lo = x * TWO_OVER_PI_LO;
    f128 z       = prod_hi + prod_lo;

    int64_t n = static_cast<int64_t>(nearbyintq(z));

    // b) Reconstruct r = x – n * (π/2) using the 3-term split
    f128 r = ((x - n * PI_OVER_2_HI)
                - n * PI_OVER_2_MI)
                - n * PI_OVER_2_LO;

    // c) Strict range folding into [-π/4, π/4] by adjusting n directly
    static constexpr f128 PI_4 = M_PI_4q; // π/4
    static constexpr f128 PI_2 = M_PI_2q; // π/2

    if (r > PI_4) {
        r -= PI_2;
        n += 1;
    }
    else if (r < -PI_4) {
        r += PI_2;
        n -= 1;
    }

    // d) Determine final quadrant q from adjusted n
    int64_t q = n % 4;
    if (q < 0) q += 4;

    // e) Select kernel and sign based on q for sine:
    //    q = 0 :  sin(r)
    //    q = 1 :  cos(r)
    //    q = 2 : -sin(r)
    //    q = 3 : -cos(r)
    bool useSin = (q == 0) || (q == 2);
    bool negate = (q >= 2);

    f128 y = useSin ? sin_kernel(r) : cos_kernel(r);
    return negate ? -y : y;
}
