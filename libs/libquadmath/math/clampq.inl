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

/** \brief Return a value that ranges from "lo" to "hi", including the values of both "lo" and "hi" as potential output values.
 *
 * \param v f128 The input value, to be limited to being between the other two parameters.
 * \param lo f128 The lower limit.  If the input value "v" is below this value, this value will be returned instead.
 * \param hi f128 The upper limit.  If the input value "v" is above this value, this value will be returned instead.
 * \return f128 The input value "v", but limited to the range of the input values "lo" to "hi".
 *
 * Return a value that ranges from "lo" to "hi", including the values of both "lo" to "hi" as potential output values.  128-bit float version.
 * This is pretty much a combination of min() and max() functions combined into one, nothing fancy here.
 */
static HOT_INLINE f128 clampq(const f128 v, const f128 lo, const f128 hi) noexcept
{
    if (invalidq(v) || invalidq(lo) || invalidq(hi)) return NANq;

    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

/** \brief Return a value that ranges from 0.0 to 1.0, including the values of both 0.0 and 1.0 as potential output values.
 *
 * \param v f128 The input value, to be limited to being between 0.0 and 1.0.
 * \return f128 The input value "v", but limited.
 *
 * This is like the regular version, clampq(), but this version assumes the upper and lower limits are 1.0 and 0.0, respectively.
 */
static HOT_INLINE f128 clamp01q(const f128 v) noexcept
{
    if (invalidq(v)) return NANq;

    return (v >= 0.q) ? ((v <= 1.q) ? v : 1.q) : 0.q;
}



/** \brief Return a value that ranges from "lo" to "hi", including the values of both "lo" and "hi" as potential output values.
 *
 * \param v double The input value, to be limited to being between the other two parameters.
 * \param lo double The lower limit.  If the input value "v" is below this value, this value will be returned instead.
 * \param hi double The upper limit.  If the input value "v" is above this value, this value will be returned instead.
 * \return double The input value "v", but limited to the range of the input values "lo" to "hi".
 *
 * This function is a 64-bit float version.
 * Return a value that ranges from "lo" to "hi", including the values of both "lo" to "hi" as potential output values.  128-bit float version.
 * This is pretty much a combination of min() and max() functions combined into one, nothing fancy here.
 */
static HOT_INLINE f64 clamp(const f64 v, const f64 lo, const f64 hi) noexcept
{
    if (invalid(v) || invalid(lo) || invalid(hi)) return NAN;

    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

/** \brief Return a value that ranges from 0.0 to 1.0, including the values of both 0.0 and 1.0 as potential output values.
 *
 * \param v f64 The input value, to be limited to being between 0.0 and 1.0.
 * \return f64 The input value "v", but limited.
 *
 * This function is a 64-bit float version.
 * This is like the regular version, clampq(), but this version assumes the upper and lower limits are 1.0 and 0.0, respectively.
 */
static HOT_INLINE f64 clamp01(const f64 v) noexcept
{
    if (invalid(v)) return NAN;

    return (v >= 0.0) ? ((v <= 1.0) ? v : 1.0) : 0.0;
}



/** \brief Return a value that ranges from "lo" to "hi", including the values of both "lo" and "hi" as potential output values.
 *
 * \param v f32 The input value, to be limited to being between the other two parameters.
 * \param lo f32 The lower limit.  If the input value "v" is below this value, this value will be returned instead.
 * \param hi f32 The upper limit.  If the input value "v" is above this value, this value will be returned instead.
 * \return f32 The input value "v", but limited to the range of the input values "lo" to "hi".
 *
 * This function is a 32-bit float version.
 * Return a value that ranges from "lo" to "hi", including the values of both "lo" to "hi" as potential output values.  128-bit float version.
 * This is pretty much a combination of min() and max() functions combined into one, nothing fancy here.
 */
static HOT_INLINE f32 clampf(const f32 v, const f32 lo, const f32 hi) noexcept
{
    if (invalidf(v) || invalidf(lo) || invalidf(hi)) return NANf;

    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

/** \brief Return a value that ranges from 0.0 to 1.0, including the values of both 0.0 and 1.0 as potential output values.
 *
 * \param v f32 The input value, to be limited to being between 0.0 and 1.0.
 * \return f32 The input value "v", but limited.
 *
 * This function is a 32-bit float version.
 * This is like the regular version, clampq(), but this version assumes the upper and lower limits are 1.0 and 0.0, respectively.
 */
static HOT_INLINE f32 clamp01f(const f32 v) noexcept
{
    if (invalidf(v)) return NANf;

    return (v >= 0.f) ? ((v <= 1.f) ? v : 1.f) : 0.f;
}



/** \brief Return a value that ranges from "lo" to "hi", including the values of both "lo" and "hi" as potential output values.
 *
 * \param v f16 The input value, to be limited to being between the other two parameters.
 * \param lo f16 The lower limit.  If the input value "v" is below this value, this value will be returned instead.
 * \param hi f16 The upper limit.  If the input value "v" is above this value, this value will be returned instead.
 * \return f16 The input value "v", but limited to the range of the input values "lo" to "hi".
 *
 * This function is a 16-bit float version.
 * Return a value that ranges from "lo" to "hi", including the values of both "lo" to "hi" as potential output values.  128-bit float version.
 * This is pretty much a combination of min() and max() functions combined into one, nothing fancy here.
 */
static HOT_INLINE f16 clampfs(const f16 v, const f16 lo, const f16 hi) noexcept
{
    if (invalidfs(v) || invalidfs(lo) || invalidfs(hi)) return NANfs;

    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

/** \brief Return a value that ranges from 0.0 to 1.0, including the values of both 0.0 and 1.0 as potential output values.
 *
 * \param v f16 The input value, to be limited to being between 0.0 and 1.0.
 * \return f16 The input value "v", but limited.
 *
 * This function is a 16-bit float version.
 * This is like the regular version, clampq(), but this version assumes the upper and lower limits are 1.0 and 0.0, respectively.
 */
static HOT_INLINE f16 clamp01fs(const f16 v) noexcept
{
    if (invalidfs(v)) return NANfs;

    return (v >= 0.f16) ? ((v <= 1.f16) ? v : 1.f16) : 0.f16;
}
