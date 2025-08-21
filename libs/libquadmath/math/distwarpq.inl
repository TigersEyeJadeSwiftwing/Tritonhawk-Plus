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
/** \brief Returns a floating-point input value of any size that is restricted to an output range of -1.0 to 1.0.
 *
 * \param a __float128 The input value, can be any real floating-point number.
 * \return __float128 The output value, which is the input limited on a curve to -1.0 to 1.0.
 *
 * Returns a floating-point input value of any size that is restricted to an output range of -1.0 to 1.0.
 * The restriction, or amount the input value is divided by, starts at 1.0 when the input is at 0.0, and
 * increases more and more as the input value is increasingly larger and larger, whether positive or negative
 * in how far away it gets from 0.0.  At very high positive and very large negative values, going farther away
 * from zero makes less difference to the output, and the resulting output value never completely "gets there"
 * to 1.0 or -1.0, but instead gets closer and closer to such.  Useful for limiting a value to being between
 * -1.0 and 1.0 but without a hard-edge that comes from a simple min() or max() function, this function does
 * it with a curve on a graph instead.  Used in audio engineering plug-ins, pro-audio, certain things for
 * video games, and other fun stuff...
 */
static HOT_INLINE __float128 distwarpq (__float128 a)
{
    if (__builtin_isnan(a)) return NANq;
    if (__builtin_isinf(a)) return a;

    return a / ( ((a >= 0.q) ? a : -a) + 1.q );
}
#else
/** \brief Returns a floating-point input value of any size that is restricted to an output range of -1.0 to 1.0.
 *
 * \param a long double The input value, can be any real floating-point number.
 * \return long double The output value, which is the input limited on a curve to -1.0 to 1.0.
 *
 * This function is a version that uses long double input and output data types.
 * Returns a floating-point input value of any size that is restricted to an output range of -1.0 to 1.0.
 * The restriction, or amount the input value is divided by, starts at 1.0 when the input is at 0.0, and
 * increases more and more as the input value is increasingly larger and larger, whether positive or negative
 * in how far away it gets from 0.0.  At very high positive and very large negative values, going farther away
 * from zero makes less difference to the output, and the resulting output value never completely "gets there"
 * to 1.0 or -1.0, but instead gets closer and closer to such.  Useful for limiting a value to being between
 * -1.0 and 1.0 but without a hard-edge that comes from a simple min() or max() function, this function does
 * it with a curve on a graph instead.  Used in audio engineering plug-ins, pro-audio, certain things for
 * video games, and other fun stuff...
 */
static HOT_INLINE long double distwarpq (long double a)
{
    if (__builtin_isnan(a)) return NANq;
    if (__builtin_isinf(a)) return a;

    return a / ( ((a >= 0.L) ? a : -a) + 1.L );
}
#endif

/** \brief Returns a floating-point input value of any size that is restricted to an output range of -1.0 to 1.0.
 *
 * \param a float The input value, can be any real floating-point number.
 * \return float The output value, which is the input limited on a curve to -1.0 to 1.0.
 *
 * This function is a 32-bit float version.
 * Returns a floating-point input value of any size that is restricted to an output range of -1.0 to 1.0.
 * The restriction, or amount the input value is divided by, starts at 1.0 when the input is at 0.0, and
 * increases more and more as the input value is increasingly larger and larger, whether positive or negative
 * in how far away it gets from 0.0.  At very high positive and very large negative values, going farther away
 * from zero makes less difference to the output, and the resulting output value never completely "gets there"
 * to 1.0 or -1.0, but instead gets closer and closer to such.  Useful for limiting a value to being between
 * -1.0 and 1.0 but without a hard-edge that comes from a simple min() or max() function, this function does
 * it with a curve on a graph instead.  Used in audio engineering plug-ins, pro-audio, certain things for
 * video games, and other fun stuff...
 */
static HOT_INLINE float distwarpf (float a)
{
    if (__builtin_isnan(a)) return NANq;
    if (__builtin_isinf(a)) return a;

    return a / ( ((a >= 0.f) ? a : -a) + 1.f );
}

/** \brief Returns a floating-point input value of any size that is restricted to an output range of -1.0 to 1.0.
 *
 * \param a double The input value, can be any real floating-point number.
 * \return double The output value, which is the input limited on a curve to -1.0 to 1.0.
 *
 * This function is a 64-bit float version.
 * Returns a floating-point input value of any size that is restricted to an output range of -1.0 to 1.0.
 * The restriction, or amount the input value is divided by, starts at 1.0 when the input is at 0.0, and
 * increases more and more as the input value is increasingly larger and larger, whether positive or negative
 * in how far away it gets from 0.0.  At very high positive and very large negative values, going farther away
 * from zero makes less difference to the output, and the resulting output value never completely "gets there"
 * to 1.0 or -1.0, but instead gets closer and closer to such.  Useful for limiting a value to being between
 * -1.0 and 1.0 but without a hard-edge that comes from a simple min() or max() function, this function does
 * it with a curve on a graph instead.  Used in audio engineering plug-ins, pro-audio, certain things for
 * video games, and other fun stuff...
 */
static HOT_INLINE double distwarp (double a)
{
    if (__builtin_isnan(a)) return NANq;
    if (__builtin_isinf(a)) return a;

    return a / ( ((a >= 0.0) ? a : -a) + 1.0 );
}
