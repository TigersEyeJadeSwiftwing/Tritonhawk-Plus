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
    #include "isinfq.inl"
    #include "isnanq.inl"
    #include "powq.inl"
#endif

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
/** \brief Lerp function, basic, linear version that calculates a blend of one floating point number and another.
 *
 * \param a __float128 Value to have all, none, or some of determine the output value.
 * \param b __float128 Value to have all, none, or some of determine the output value.
 * \param lerp_factor __float128 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \return __float128 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE __float128 lerpq(__float128 a, __float128 b, __float128 lerp_factor)
{
    if (isnanq(a) || isnanq(b) || isnanq(lerp_factor)) return NANq;
    if (isinfq(a) || isinfq(b) || isinfq(lerp_factor)) return NANq;

    if (lerp_factor == 0.q) return a;
    if (lerp_factor == 1.q) return b;
    if (lerp_factor == 0.5q) return (a + b) * 0.5q;

    __float128 lerp_fac = (lerp_factor > 1.q) ? 1.q : (lerp_factor < 0.q) ? 0.q : lerp_factor;

    return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a __float128 Value to have all, none, or some of determine the output value.
 * \param b __float128 Value to have all, none, or some of determine the output value.
 * \param lerp_factor __float128 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent __float128 The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerp_factor inwards towards 0.5, while values below 1.0 pull the lerping blend factor out away from 0.5.
 * \return __float128 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE __float128 lerp_exp_inq(__float128 a, __float128 b, __float128 lerp_factor, __float128 exponent)
{
    if (isnanq(a) || isnanq(b) || isnanq(lerp_factor) || isnanq(exponent)) return NANq;
    if (isinfq(a) || isinfq(b) || isinfq(lerp_factor) || isinfq(exponent)) return NANq;

    if (exponent <= 0.q) return (a + b) * 0.5q;

    if (lerp_factor == 0.q) return a;
    if (lerp_factor == 1.q) return b;
    if (lerp_factor == 0.5q) return (a + b) * 0.5q;

    __float128 clamped_factor = (lerp_factor > 1.q) ? 1.q : (lerp_factor < 0.q) ? 0.q : lerp_factor;

    __float128 factor0 = (clamped_factor < 0.5q) ?
        (0.5q - clamped_factor) * 2.0q :
        (clamped_factor - 0.5q) * 2.0q ;
    __float128 factor1 = powq(factor0, 1.q / exponent);
    __float128 lerp_fac = (clamped_factor < 0.5q) ?
        0.5q - (factor1 * 0.5q) :
        0.5q + (factor1 * 0.5q) ;
    return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a __float128 Value to have all, none, or some of determine the output value.
 * \param b __float128 Value to have all, none, or some of determine the output value.
 * \param lerp_factor __float128 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent __float128 The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerping blend factor out away from 0.5, while values below 1.0 pull the lerp_factor inwards towards 0.5.
 * \return __float128 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE __float128 lerp_exp_outq(__float128 a, __float128 b, __float128 lerp_factor, __float128 exponent)
{
    if (isnanq(a) || isnanq(b) || isnanq(lerp_factor) || isnanq(exponent)) return NANq;
    if (isinfq(a) || isinfq(b) || isinfq(lerp_factor) || isinfq(exponent)) return NANq;

    if (lerp_factor == 0.q) return a;
    if (lerp_factor == 1.q) return b;
    if (lerp_factor == 0.5q) return (a + b) * 0.5q;

    __float128 clamped_factor = (lerp_factor > 1.q) ? 1.q : (lerp_factor < 0.q) ? 0.q : lerp_factor;

    if (exponent <= 0.q) return (clamped_factor < 0.5q) ? a : (clamped_factor < 0.5q) ? b : (a * (1.0q - clamped_factor)) + (b * clamped_factor);

    __float128 factor0 = (clamped_factor < 0.5q) ?
        (0.5q - clamped_factor) * 2.0q :
        (clamped_factor - 0.5q) * 2.0q ;
    __float128 factor1 = powq(factor0, exponent);
    __float128 lerp_fac = (clamped_factor < 0.5q) ?
        0.5q - (factor1 * 0.5q) :
        0.5q + (factor1 * 0.5q) ;

    return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
}

#else
/** \brief Lerp function, basic, linear version that calculates a blend of one floating point number and another.
 *
 * \param a long double Value to have all, none, or some of determine the output value.
 * \param b long double Value to have all, none, or some of determine the output value.
 * \param lerp_factor long double Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \return long double Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE long double lerpq(long double a, long double b, long double lerp_factor)
{
    if (isnanq(a) || isnanq(b) || isnanq(lerp_factor)) return NANq;
    if (isinfq(a) || isinfq(b) || isinfq(lerp_factor)) return NANq;

    if (lerp_factor == 0.L) return a;
    if (lerp_factor == 1.L) return b;
    if (lerp_factor == 0.5L) return (a + b) * 0.5L;

    long double lerp_fac = (lerp_factor > 1.L) ? 1.L : (lerp_factor < 0.L) ? 0.L : lerp_factor;

    return (a * (1.0L - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a long double Value to have all, none, or some of determine the output value.
 * \param b long double Value to have all, none, or some of determine the output value.
 * \param lerp_factor long double Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent long double The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerp_factor inwards towards 0.5, while values below 1.0 pull the lerping blend factor out away from 0.5.
 * \return long double Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE long double lerp_exp_inq(long double a, long double b, long double lerp_factor, long double exponent)
{
    if (isnanq(a) || isnanq(b) || isnanq(lerp_factor) || isnanq(exponent)) return NANq;
    if (isinfq(a) || isinfq(b) || isinfq(lerp_factor) || isinfq(exponent)) return NANq;

    if (exponent <= 0.L) return (a + b) * 0.5L;

    if (lerp_factor == 0.L) return a;
    if (lerp_factor == 1.L) return b;
    if (lerp_factor == 0.5L) return (a + b) * 0.5L;

    long double clamped_factor = (lerp_factor > 1.L) ? 1.L : (lerp_factor < 0.L) ? 0.L : lerp_factor;

    long double factor0 = (clamped_factor < 0.5L) ?
        (0.5L - clamped_factor) * 2.0L :
        (clamped_factor - 0.5L) * 2.0L ;
    long double factor1 = powl(factor0, 1.L / exponent);
    long double lerp_fac = (clamped_factor < 0.5L) ?
        0.5L - (factor1 * 0.5L) :
        0.5L + (factor1 * 0.5L) ;
    return (a * (1.0L - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a long double Value to have all, none, or some of determine the output value.
 * \param b long double Value to have all, none, or some of determine the output value.
 * \param lerp_factor long double Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent long double The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerping blend factor out away from 0.5, while values below 1.0 pull the lerp_factor inwards towards 0.5.
 * \return long double Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE long double lerp_exp_outq(long double a, long double b, long double lerp_factor, long double exponent)
{
    if (isnanq(a) || isnanq(b) || isnanq(lerp_factor) || isnanq(exponent)) return NANq;
    if (isinfq(a) || isinfq(b) || isinfq(lerp_factor) || isinfq(exponent)) return NANq;

    if (lerp_factor == 0.L) return a;
    if (lerp_factor == 1.L) return b;
    if (lerp_factor == 0.5L) return (a + b) * 0.5L;

    long double clamped_factor = (lerp_factor > 1.L) ? 1.L : (lerp_factor < 0.L) ? 0.L : lerp_factor;

    if (exponent <= 0.L) return (clamped_factor < 0.5L) ? a : (clamped_factor < 0.5L) ? b : (a * (1.0L - clamped_factor)) + (b * clamped_factor);

    long double factor0 = (clamped_factor < 0.5L) ?
        (0.5L - clamped_factor) * 2.0L :
        (clamped_factor - 0.5L) * 2.0L ;
    long double factor1 = powl(factor0, exponent);
    long double lerp_fac = (clamped_factor < 0.5L) ?
        0.5L - (factor1 * 0.5L) :
        0.5L + (factor1 * 0.5L) ;

    return (a * (1.0L - lerp_fac)) + (b * lerp_fac);
}
#endif

/** \brief Lerp function, basic, linear version that calculates a blend of one floating point number and another.
 *
 * \param a float Value to have all, none, or some of determine the output value.
 * \param b float Value to have all, none, or some of determine the output value.
 * \param lerp_factor float Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \return float Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE float lerpf(float a, float b, float lerp_factor)
{
    if (isnanq(a) || isnanq(b) || isnanq(lerp_factor)) return NAN;
    if (isinfq(a) || isinfq(b) || isinfq(lerp_factor)) return NAN;

    if (lerp_factor == 0.f) return a;
    if (lerp_factor == 1.f) return b;
    if (lerp_factor == 0.5f) return (a + b) * 0.5f;

    float lerp_fac = (lerp_factor > 1.f) ? 1.f : (lerp_factor < 0.f) ? 0.f : lerp_factor;

    return (a * (1.0f - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a float Value to have all, none, or some of determine the output value.
 * \param b float Value to have all, none, or some of determine the output value.
 * \param lerp_factor float Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent float The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerp_factor inwards towards 0.5, while values below 1.0 pull the lerping blend factor out away from 0.5.
 * \return float Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE float lerp_exp_inf(float a, float b, float lerp_factor, float exponent)
{
    if (isnanq(a) || isnanq(b) || isnanq(lerp_factor) || isnanq(exponent)) return NAN;
    if (isinfq(a) || isinfq(b) || isinfq(lerp_factor) || isinfq(exponent)) return NAN;

    if (exponent <= 0.f) return (a + b) * 0.5f;

    if (lerp_factor == 0.f) return a;
    if (lerp_factor == 1.f) return b;
    if (lerp_factor == 0.5f) return (a + b) * 0.5f;

    float clamped_factor = (lerp_factor > 1.f) ? 1.f : (lerp_factor < 0.f) ? 0.f : lerp_factor;

    float factor0 = (clamped_factor < 0.5f) ?
        (0.5f - clamped_factor) * 2.0f :
        (clamped_factor - 0.5f) * 2.0f ;
    float factor1 = powf(factor0, 1.f / exponent);
    float lerp_fac = (clamped_factor < 0.5f) ?
        0.5f - (factor1 * 0.5f) :
        0.5f + (factor1 * 0.5f) ;
    return (a * (1.0f - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a float Value to have all, none, or some of determine the output value.
 * \param b float Value to have all, none, or some of determine the output value.
 * \param lerp_factor float Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent float The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerping blend factor out away from 0.5, while values below 1.0 pull the lerp_factor inwards towards 0.5.
 * \return float Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE float lerp_exp_outf(float a, float b, float lerp_factor, float exponent)
{
    if (isnanq(a) || isnanq(b) || isnanq(lerp_factor) || isnanq(exponent)) return NAN;
    if (isinfq(a) || isinfq(b) || isinfq(lerp_factor) || isinfq(exponent)) return NAN;

    if (lerp_factor == 0.f) return a;
    if (lerp_factor == 1.f) return b;
    if (lerp_factor == 0.5f) return (a + b) * 0.5f;

    float clamped_factor = (lerp_factor > 1.f) ? 1.f : (lerp_factor < 0.f) ? 0.f : lerp_factor;

    if (exponent <= 0.f) return (clamped_factor < 0.5f) ? a : (clamped_factor < 0.5f) ? b : (a * (1.0f - clamped_factor)) + (b * clamped_factor);

    float factor0 = (clamped_factor < 0.5f) ?
        (0.5f - clamped_factor) * 2.0f :
        (clamped_factor - 0.5f) * 2.0f ;
    float factor1 = powf(factor0, exponent);
    float lerp_fac = (clamped_factor < 0.5f) ?
        0.5f - (factor1 * 0.5f) :
        0.5f + (factor1 * 0.5f) ;

    return (a * (1.0f - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, basic, linear version that calculates a blend of one floating point number and another.
 *
 * \param a double Value to have all, none, or some of determine the output value.
 * \param b double Value to have all, none, or some of determine the output value.
 * \param lerp_factor double Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \return double Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE double lerpd(double a, double b, double lerp_factor)
{
    if (isnanq(a) || isnanq(b) || isnanq(lerp_factor)) return NAN;
    if (isinfq(a) || isinfq(b) || isinfq(lerp_factor)) return NAN;

    if (lerp_factor == 0.0) return a;
    if (lerp_factor == 1.0) return b;
    if (lerp_factor == 0.50) return (a + b) * 0.50;

    double lerp_fac = (lerp_factor > 1.0) ? 1.0 : (lerp_factor < 0.0) ? 0.0 : lerp_factor;

    return (a * (1.00 - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a double Value to have all, none, or some of determine the output value.
 * \param b double Value to have all, none, or some of determine the output value.
 * \param lerp_factor double Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent double The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerp_factor inwards towards 0.5, while values below 1.0 pull the lerping blend factor out away from 0.5.
 * \return double Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE double lerp_exp_ind(double a, double b, double lerp_factor, double exponent)
{
    if (isnanq(a) || isnanq(b) || isnanq(lerp_factor) || isnanq(exponent)) return NAN;
    if (isinfq(a) || isinfq(b) || isinfq(lerp_factor) || isinfq(exponent)) return NAN;

    if (exponent <= 0.0) return (a + b) * 0.50;

    if (lerp_factor == 0.0) return a;
    if (lerp_factor == 1.0) return b;
    if (lerp_factor == 0.50) return (a + b) * 0.50;

    double clamped_factor = (lerp_factor > 1.0) ? 1.0 : (lerp_factor < 0.0) ? 0.0 : lerp_factor;

    double factor0 = (clamped_factor < 0.50) ?
        (0.50 - clamped_factor) * 2.00 :
        (clamped_factor - 0.50) * 2.00 ;
    double factor1 = pow(factor0, 1.0 / exponent);
    double lerp_fac = (clamped_factor < 0.50) ?
        0.50 - (factor1 * 0.50) :
        0.50 + (factor1 * 0.50) ;
    return (a * (1.00 - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a double Value to have all, none, or some of determine the output value.
 * \param b double Value to have all, none, or some of determine the output value.
 * \param lerp_factor double Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent double The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerping blend factor out away from 0.5, while values below 1.0 pull the lerp_factor inwards towards 0.5.
 * \return double Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE double lerp_exp_outd(double a, double b, double lerp_factor, double exponent)
{
    if (isnanq(a) || isnanq(b) || isnanq(lerp_factor) || isnanq(exponent)) return NAN;
    if (isinfq(a) || isinfq(b) || isinfq(lerp_factor) || isinfq(exponent)) return NAN;

    if (lerp_factor == 0.0) return a;
    if (lerp_factor == 1.0) return b;
    if (lerp_factor == 0.50) return (a + b) * 0.50;

    double clamped_factor = (lerp_factor > 1.0) ? 1.0 : (lerp_factor < 0.0) ? 0.0 : lerp_factor;

    if (exponent <= 0.0) return (clamped_factor < 0.50) ? a : (clamped_factor < 0.50) ? b : (a * (1.00 - clamped_factor)) + (b * clamped_factor);

    double factor0 = (clamped_factor < 0.50) ?
        (0.50 - clamped_factor) * 2.00 :
        (clamped_factor - 0.50) * 2.00 ;
    double factor1 = pow(factor0, exponent);
    double lerp_fac = (clamped_factor < 0.50) ?
        0.50 - (factor1 * 0.50) :
        0.50 + (factor1 * 0.50) ;

    return (a * (1.00 - lerp_fac)) + (b * lerp_fac);
}
