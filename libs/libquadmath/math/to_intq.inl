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

#include "truncq.inl"

/** \brief Returns an integer, from the 128-bit floating point input value, truncated.
 *
 * \param x f128 The input value to be made into an s32.
 * \return s32 The input value, converted into an s32.
 */
static HOT_INLINE s32 to_intq(const f128 x) noexcept
{
    if (invalidq(x)) return 0;

    return s32(truncq(x) + 0.1q);
}

/** \brief Returns an integer, from the 64-bit floating point input value, truncated.
 *
 * \param x f64 The input value to be made into an s32.
 * \return s32 The input value, converted into an s32.
 */
static HOT_INLINE s32 to_int(const f64 x) noexcept
{
    if (invalid(x)) return 0;

    return s32(trunc(x) + 0.1);
}

/** \brief Returns an integer, from the 32-bit floating point input value, truncated.
 *
 * \param x f32 The input value to be made into an s32.
 * \return s32 The input value, converted into an s32.
 */
static HOT_INLINE s32 to_intf(const f32 x) noexcept
{
    if (invalidf(x)) return 0;

    return s32(truncf(x) + 0.1f);
}

/** \brief Returns an integer, from the 16-bit floating point input value, truncated.
 *
 * \param x f16 The input value to be made into an s32.
 * \return s32 The input value, converted into an s32.
 */
static HOT_INLINE s32 to_intfs(const f16 x) noexcept
{
    if (invalidfs(x)) return 0;

    return s32((f16)truncf(x) + 0.1f16);
}
