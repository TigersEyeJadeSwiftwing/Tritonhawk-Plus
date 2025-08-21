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
/** \brief Return a value that ranges from "lo" to "hi", including the values of both "lo" and "hi" as potential output values.
 *
 * \param v __float128 The input value, to be limited to being between the other two parameters.
 * \param lo __float128 The lower limit.  If the input value "v" is below this value, this value will be returned instead.
 * \param hi __float128 The upper limit.  If the input value "v" is above this value, this value will be returned instead.
 * \return __float128 The input value "v", but limited to the range of the input values "lo" to "hi".
 *
 * Return a value that ranges from "lo" to "hi", including the values of both "lo" to "hi" as potential output values.  128-bit float version.
 * This is pretty much a combination of min() and max() functions combined into one, nothing fancy here.
 */
static HOT_INLINE __float128 clampq(__float128 v, __float128 lo, __float128 hi)
{
    if (__builtin_isnan(v) || __builtin_isnan(lo) || __builtin_isnan(hi)) return NANq;

    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}
#else
/** \brief Return a value that ranges from "lo" to "hi", including the values of both "lo" and "hi" as potential output values.
 *
 * \param v long double The input value, to be limited to being between the other two parameters.
 * \param lo long double The lower limit.  If the input value "v" is below this value, this value will be returned instead.
 * \param hi long double The upper limit.  If the input value "v" is above this value, this value will be returned instead.
 * \return long double The input value "v", but limited to the range of the input values "lo" to "hi".
 *
 * Return a value that ranges from "lo" to "hi", including the values of both "lo" to "hi" as potential output values.  128-bit float version.
 * This is pretty much a combination of min() and max() functions combined into one, nothing fancy here.
 */
static HOT_INLINE long double clampq(long double v, long double lo, long double hi)
{
    if (__builtin_isnan(v) || __builtin_isnan(lo) || __builtin_isnan(hi)) return NANq;

    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}
#endif

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
/** \brief Return a value that ranges from 0.0 to 1.0, including the values of both 0.0 and 1.0 as potential output values.
 *
 * \param v __float128 The input value, to be limited to being between 0.0 and 1.0.
 * \return __float128 The input value "v", but limited.
 *
 * This is like the regular version, clampq(), but this version assumes the upper and lower limits are 1.0 and 0.0, respectively.
 */
static HOT_INLINE __float128 clamp01q(__float128 v)
{
    if (__builtin_isnan(v)) return NANq;

    return (v >= 0.q) ? ((v <= 1.q) ? v : 1.q) : 0.q;
}
#else
/** \brief Return a value that ranges from 0.0 to 1.0, including the values of both 0.0 and 1.0 as potential output values.
 *
 * \param v long double The input value, to be limited to being between 0.0 and 1.0.
 * \return long double The input value "v", but limited.
 *
 * This is like the regular version, clampq(), but this version assumes the upper and lower limits are 1.0 and 0.0, respectively.
 */
static HOT_INLINE long double clamp01q(long double v)
{
    if (__builtin_isnan(v)) return NANq;

    return (v >= 0.L) ? ((v <= 1.L) ? v : 1.L) : 0.L;
}
#endif

/** \brief Return a value that ranges from "lo" to "hi", including the values of both "lo" and "hi" as potential output values.
 *
 * \param v float The input value, to be limited to being between the other two parameters.
 * \param lo float The lower limit.  If the input value "v" is below this value, this value will be returned instead.
 * \param hi float The upper limit.  If the input value "v" is above this value, this value will be returned instead.
 * \return float The input value "v", but limited to the range of the input values "lo" to "hi".
 *
 * This function is a 32-bit float version.
 * Return a value that ranges from "lo" to "hi", including the values of both "lo" to "hi" as potential output values.  128-bit float version.
 * This is pretty much a combination of min() and max() functions combined into one, nothing fancy here.
 */
static HOT_INLINE float clampf (float v, float lo, float hi)
{
    if (__builtin_isnan(v) || __builtin_isnan(lo) || __builtin_isnan(hi)) return NANq;

    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

/** \brief Return a value that ranges from 0.0 to 1.0, including the values of both 0.0 and 1.0 as potential output values.
 *
 * \param v float The input value, to be limited to being between 0.0 and 1.0.
 * \return float The input value "v", but limited.
 *
 * This function is a 32-bit float version.
 * This is like the regular version, clampq(), but this version assumes the upper and lower limits are 1.0 and 0.0, respectively.
 */
static HOT_INLINE float clamp01f (float v)
{
    if (__builtin_isnan(v)) return NANq;

    return (v >= 0.f) ? ((v <= 1.f) ? v : 1.f) : 0.f;
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
static HOT_INLINE double clamp (double v, double lo, double hi)
{
    if (__builtin_isnan(v) || __builtin_isnan(lo) || __builtin_isnan(hi)) return NANq;

    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

/** \brief Return a value that ranges from 0.0 to 1.0, including the values of both 0.0 and 1.0 as potential output values.
 *
 * \param v double The input value, to be limited to being between 0.0 and 1.0.
 * \return double The input value "v", but limited.
 *
 * This function is a 64-bit float version.
 * This is like the regular version, clampq(), but this version assumes the upper and lower limits are 1.0 and 0.0, respectively.
 */
static HOT_INLINE double clamp01 (double v)
{
    if (__builtin_isnan(v)) return NANq;

    return (v >= 0.0) ? ((v <= 1.0) ? v : 1.0) : 0.0;
}
