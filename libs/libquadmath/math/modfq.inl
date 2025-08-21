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

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #include "isnanq.inl"
    #include "isinfq.inl"
    #include "floorq.inl"
    #include "fabsq.inl"
#endif

static HOT_INLINE __float128 modfq(__float128 num, __float128* iptr)
{
    if (isnanq(num)) return NANq;
    if (isinfq(num))
    {
        *iptr = INFINITYq;
        return 0.q;
    }

    // Check if iptr is null
    if (iptr == nullptr)
        return num - floorq(num);

    // Get the integer part of num
    __float128 intPart = floorq(num);

    // Store the integer part in the location pointed by iptr
    *iptr = intPart;

    // Calculate the modulus
    __float128 modResult = num - intPart;

    // Ensure the result is non-negative
    if (modResult < 0) {
        modResult += fabsq(intPart);
    }

    return modResult;
}
