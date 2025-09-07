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
#include "fmaq.inl"

/** \brief 128-bit float sine function.
 *
 * \param x f128 Input value.
 * \return f128 Output value, sine of input.
 */
static HOT_INLINE f128 sinq(const f128 x) noexcept
{
    // -- a) Handle bad input
    if (invalidq(x)) return x;

    // -- b) Argument reduction: n = round(x * 2/π) --
    // split product with TWO_OVER_PI_HI / _LO
    f128 z_hi = x * TWO_OVER_PI_HI;
    f128 z_lo = x * TWO_OVER_PI_LO;

    // ALIGN must be 2^112
    static constexpr f128 ALIGN = 0x1.0p112q;

    // now the sum really picks up both hi and lo parts
    f128 z = z_hi + z_lo * ALIGN;
    int64_t    n = (int64_t)nearbyintq(z);

    // -- c) Reconstruct r = x – n*(π/2) using the 3-term split --
    f128 r = ((x - n*PI_OVER_2_HI)
                  - n*PI_OVER_2_MI)
                  - n*PI_OVER_2_LO;

    // -- d) Which quadrant? sign? sine or cosine? --
    int      qd     = n & 3;
    bool     useSin = (qd & 1) == 0;  // even quadrants → sin
    bool     neg    = (qd == 2 || qd == 3);  // flip sign in 2nd and 3rd

    // -- e) Fold |r| into ≤ π/4 by reflecting around π/4 --
    static constexpr f128 PIO4 = M_PI_2q * 0.5q;
    if (r >  PIO4) {
        r       = M_PI_2q - r;
        useSin  = !useSin;
    }
    else if (r < -PIO4) {
        r       = -M_PI_2q - r;
        useSin  = !useSin;
        neg     = !neg;
    }

    // -- f) Evaluate the right kernel and apply sign --
    f128 y = useSin ? sin_kernel(r) : cos_kernel(r);
    return neg ? -y : y;
}
