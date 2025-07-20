#pragma once

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

/** \brief Return a value that ranges from "lo" to "hi", including the values of both "lo" and "hi" as potential output values.
 *
 * \param v (__float128) The input value, to be limited to being between the other two parameters.
 * \param lo (__float128) The lower limit.  If the input value "v" is below this value, this value will be returned instead.
 * \param hi (__float128) The upper limit.  If the input value "v" is above this value, this value will be returned instead.
 * \return (__float128) The input value "v", but limited to the range of the input values "lo" to "hi".
 *
 * Return a value that ranges from "lo" to "hi", including the values of both "lo" to "hi" as potential output values.  128-bit float version.
 * This is pretty much a combination of min() and max() functions combined into one, nothing fancy here.
 * If you have trouble understanding it, and you're not brand-new to C or C-style languages, you may want to reconsider being a C/C++ programmer...
 */
#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
static inline __attribute__((always_inline, hot))
__float128 clampq(__float128 v, __float128 lo, __float128 hi)
{
    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}
#else
static inline __attribute__((always_inline, hot))
long double clampq(long double v, long double lo, long double hi)
{
    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}
#endif

/** \brief Return a value that ranges from 0.0 to 1.0, including the values of both 0.0 and 1.0 as potential output values.
 *
 * \param v (__float128) The input value, to be limited to being between 0.0 and 1.0.
 * \return (__float128) The input value "v", but limited.
 *
 * This is like the regular version, clampq(), but this version assumes the upper and lower limits are 1.0 and 0.0, respectively.
 */
#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
static inline __attribute__((always_inline, hot))
__float128 clamp01q(__float128 v)
{
    return (v >= 0.q) ? ((v <= 1.q) ? v : 1.q) : 0.q;
}
#else
static inline __attribute__((always_inline, hot))
long double clamp01q(long double v)
{
    return (v >= 0.L) ? ((v <= 1.L) ? v : 1.L) : 0.L;
}
#endif

static inline __attribute__((always_inline, hot))
float clampf (float v, float lo, float hi)
{
    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

static inline __attribute__((always_inline, hot))
float clamp01f (float v)
{
    return (v >= 0.f) ? ((v <= 1.f) ? v : 1.f) : 0.f;
}

static inline __attribute__((always_inline, hot))
double clampd (double v, double lo, double hi)
{
    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

static inline __attribute__((always_inline, hot))
double clamp01d (double v)
{
    return (v >= 0.0) ? ((v <= 1.0) ? v : 1.0) : 0.0;
}
