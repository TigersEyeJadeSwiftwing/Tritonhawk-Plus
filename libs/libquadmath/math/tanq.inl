#pragma once

#include "sinq.inl"
#include "cosq.inl"

/*
    Copyright (c) Tiger's Eye Jade Swiftwing, all rights reserved.
    This file is written by Tiger's Eye Jade Swiftwing.  It is licensed under the
GPLv3 license.  Note that my first name is "Tiger's Eye" (which is two words), my
middle name is "Jade", and "Swiftwing" is one word that is my last name.
    This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.  This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.  You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>.
*/

static inline __attribute__((always_inline, hot))
__float128 tanq(__float128 x)
{
    // Handle NaN/Inf
    if (x != x || x == INFINITYq || x == INFINITYnq) return NANq;

    // Compute sine and cosine via our high-precision routines
    __float128 s = sinq(x);
    __float128 c = cosq(x);

    // Divide, taking care of poles
    // If |c| is extremely small, result overflows to ±Inf
    if (c == 0.q)
        return (s > 0.q) ? INFINITYq : INFINITYnq;

    return s / c;
}
