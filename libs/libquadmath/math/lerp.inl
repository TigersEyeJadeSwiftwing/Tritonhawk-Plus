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

#include "powq.inl"

/** \brief Lerp function, basic, linear version that calculates a blend of one floating point number and another.
 *
 * \param a f128 Value to have all, none, or some of determine the output value.
 * \param b f128 Value to have all, none, or some of determine the output value.
 * \param lerp_factor f128 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \return f128 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE f128 lerpq(const f128 a, const f128 b, const f128 lerp_factor) noexcept
{
    if (invalidq(a) || invalidq(b) || invalidq(lerp_factor)) return NANq;

    if (lerp_factor == 0.q) return a;
    if (lerp_factor == 1.q) return b;
    if (lerp_factor == 0.5q) return (a + b) * 0.5q;

    f128 lerp_fac = (lerp_factor > 1.q) ? 1.q : (lerp_factor < 0.q) ? 0.q : lerp_factor;

    return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a f128 Value to have all, none, or some of determine the output value.
 * \param b f128 Value to have all, none, or some of determine the output value.
 * \param lerp_factor f128 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent f128 The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerp_factor inwards towards 0.5, while values below 1.0 pull the lerping blend factor out away from 0.5.
 * \return f128 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE f128 lerp_exp_inq(const f128 a, const f128 b, const f128 lerp_factor, const f128 exponent) noexcept
{
    if (invalidq(a) || invalidq(b) || invalidq(lerp_factor) || invalidq(exponent)) return NANq;

    if (exponent <= 0.q) return (a + b) * 0.5q;

    if (lerp_factor == 0.q) return a;
    if (lerp_factor == 1.q) return b;
    if (lerp_factor == 0.5q) return (a + b) * 0.5q;

    f128 clamped_factor = (lerp_factor > 1.q) ? 1.q : (lerp_factor < 0.q) ? 0.q : lerp_factor;

    f128 factor0 = (clamped_factor < 0.5q) ?
        (0.5q - clamped_factor) * 2.0q :
        (clamped_factor - 0.5q) * 2.0q ;
    f128 factor1 = powq(factor0, 1.q / exponent);
    f128 lerp_fac = (clamped_factor < 0.5q) ?
        0.5q - (factor1 * 0.5q) :
        0.5q + (factor1 * 0.5q) ;
    return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a f128 Value to have all, none, or some of determine the output value.
 * \param b f128 Value to have all, none, or some of determine the output value.
 * \param lerp_factor f128 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent f128 The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerping blend factor out away from 0.5, while values below 1.0 pull the lerp_factor inwards towards 0.5.
 * \return f128 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE f128 lerp_exp_outq(const f128 a, const f128 b, const f128 lerp_factor, const f128 exponent) noexcept
{
    if (invalidq(a) || invalidq(b) || invalidq(lerp_factor) || invalidq(exponent)) return NANq;

    if (lerp_factor == 0.q) return a;
    if (lerp_factor == 1.q) return b;
    if (lerp_factor == 0.5q) return (a + b) * 0.5q;

    f128 clamped_factor = (lerp_factor > 1.q) ? 1.q : (lerp_factor < 0.q) ? 0.q : lerp_factor;

    if (exponent <= 0.q) return (clamped_factor < 0.5q) ? a : (clamped_factor < 0.5q) ? b : (a * (1.0q - clamped_factor)) + (b * clamped_factor);

    f128 factor0 = (clamped_factor < 0.5q) ?
        (0.5q - clamped_factor) * 2.0q :
        (clamped_factor - 0.5q) * 2.0q ;
    f128 factor1 = powq(factor0, exponent);
    f128 lerp_fac = (clamped_factor < 0.5q) ?
        0.5q - (factor1 * 0.5q) :
        0.5q + (factor1 * 0.5q) ;

    return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
}



/** \brief Lerp function, basic, linear version that calculates a blend of one floating point number and another.
 *
 * \param a f64 Value to have all, none, or some of determine the output value.
 * \param b f64 Value to have all, none, or some of determine the output value.
 * \param lerp_factor f64 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \return f64 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE f64 lerpd(const f64 a, const f64 b, const f64 lerp_factor) noexcept
{
    if (invalid(a) || invalid(b) || invalid(lerp_factor)) return NAN;

    if (lerp_factor == 0.0) return a;
    if (lerp_factor == 1.0) return b;
    if (lerp_factor == 0.50) return (a + b) * 0.50;

    f64 lerp_fac = (lerp_factor > 1.0) ? 1.0 : (lerp_factor < 0.0) ? 0.0 : lerp_factor;

    return (a * (1.00 - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a f64 Value to have all, none, or some of determine the output value.
 * \param b f64 Value to have all, none, or some of determine the output value.
 * \param lerp_factor f64 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent f64 The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerp_factor inwards towards 0.5, while values below 1.0 pull the lerping blend factor out away from 0.5.
 * \return f64 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE f64 lerp_exp_ind(const f64 a, const f64 b, const f64 lerp_factor, const f64 exponent) noexcept
{
    if (invalid(a) || invalid(b) || invalid(lerp_factor) || invalid(exponent)) return NAN;

    if (exponent <= 0.0) return (a + b) * 0.50;

    if (lerp_factor == 0.0) return a;
    if (lerp_factor == 1.0) return b;
    if (lerp_factor == 0.50) return (a + b) * 0.50;

    f64 clamped_factor = (lerp_factor > 1.0) ? 1.0 : (lerp_factor < 0.0) ? 0.0 : lerp_factor;

    f64 factor0 = (clamped_factor < 0.50) ?
        (0.50 - clamped_factor) * 2.00 :
        (clamped_factor - 0.50) * 2.00 ;
    f64 factor1 = pow(factor0, 1.0 / exponent);
    f64 lerp_fac = (clamped_factor < 0.50) ?
        0.50 - (factor1 * 0.50) :
        0.50 + (factor1 * 0.50) ;
    return (a * (1.00 - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a f64 Value to have all, none, or some of determine the output value.
 * \param b f64 Value to have all, none, or some of determine the output value.
 * \param lerp_factor f64 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent f64 The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerping blend factor out away from 0.5, while values below 1.0 pull the lerp_factor inwards towards 0.5.
 * \return f64 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE f64 lerp_exp_outd(const f64 a, const f64 b, const f64 lerp_factor, const f64 exponent) noexcept
{
    if (invalid(a) || invalid(b) || invalid(lerp_factor) || invalid(exponent)) return NAN;

    if (lerp_factor == 0.0) return a;
    if (lerp_factor == 1.0) return b;
    if (lerp_factor == 0.50) return (a + b) * 0.50;

    f64 clamped_factor = (lerp_factor > 1.0) ? 1.0 : (lerp_factor < 0.0) ? 0.0 : lerp_factor;

    if (exponent <= 0.0) return (clamped_factor < 0.50) ? a : (clamped_factor < 0.50) ? b : (a * (1.00 - clamped_factor)) + (b * clamped_factor);

    f64 factor0 = (clamped_factor < 0.50) ?
        (0.50 - clamped_factor) * 2.00 :
        (clamped_factor - 0.50) * 2.00 ;
    f64 factor1 = pow(factor0, exponent);
    f64 lerp_fac = (clamped_factor < 0.50) ?
        0.50 - (factor1 * 0.50) :
        0.50 + (factor1 * 0.50) ;

    return (a * (1.00 - lerp_fac)) + (b * lerp_fac);
}



/** \brief Lerp function, basic, linear version that calculates a blend of one floating point number and another.
 *
 * \param a f32 Value to have all, none, or some of determine the output value.
 * \param b f32 Value to have all, none, or some of determine the output value.
 * \param lerp_factor f32 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \return f32 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE f32 lerpf(const f32 a, const f32 b, const f32 lerp_factor) noexcept
{
    if (invalidf(a) || invalidf(b) || invalidf(lerp_factor)) return NANf;

    if (lerp_factor == 0.f) return a;
    if (lerp_factor == 1.f) return b;
    if (lerp_factor == 0.5f) return (a + b) * 0.5f;

    f32 lerp_fac = (lerp_factor > 1.f) ? 1.f : (lerp_factor < 0.f) ? 0.f : lerp_factor;

    return (a * (1.0f - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a f32 Value to have all, none, or some of determine the output value.
 * \param b f32 Value to have all, none, or some of determine the output value.
 * \param lerp_factor f32 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent f32 The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerp_factor inwards towards 0.5, while values below 1.0 pull the lerping blend factor out away from 0.5.
 * \return f32 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE f32 lerp_exp_inf(const f32 a, const f32 b, const f32 lerp_factor, const f32 exponent) noexcept
{
    if (invalidf(a) || invalidf(b) || invalidf(lerp_factor) || invalidf(exponent)) return NANf;

    if (exponent <= 0.f) return (a + b) * 0.5f;

    if (lerp_factor == 0.f) return a;
    if (lerp_factor == 1.f) return b;
    if (lerp_factor == 0.5f) return (a + b) * 0.5f;

    f32 clamped_factor = (lerp_factor > 1.f) ? 1.f : (lerp_factor < 0.f) ? 0.f : lerp_factor;

    f32 factor0 = (clamped_factor < 0.5f) ?
        (0.5f - clamped_factor) * 2.0f :
        (clamped_factor - 0.5f) * 2.0f ;
    f32 factor1 = powf(factor0, 1.f / exponent);
    f32 lerp_fac = (clamped_factor < 0.5f) ?
        0.5f - (factor1 * 0.5f) :
        0.5f + (factor1 * 0.5f) ;
    return (a * (1.0f - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a f32 Value to have all, none, or some of determine the output value.
 * \param b f32 Value to have all, none, or some of determine the output value.
 * \param lerp_factor f32 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent f32 The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerping blend factor out away from 0.5, while values below 1.0 pull the lerp_factor inwards towards 0.5.
 * \return f32 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE f32 lerp_exp_outf(const f32 a, const f32 b, const f32 lerp_factor, const f32 exponent) noexcept
{
    if (invalidf(a) || invalidf(b) || invalidf(lerp_factor) || invalidf(exponent)) return NANf;

    if (lerp_factor == 0.f) return a;
    if (lerp_factor == 1.f) return b;
    if (lerp_factor == 0.5f) return (a + b) * 0.5f;

    f32 clamped_factor = (lerp_factor > 1.f) ? 1.f : (lerp_factor < 0.f) ? 0.f : lerp_factor;

    if (exponent <= 0.f) return (clamped_factor < 0.5f) ? a : (clamped_factor < 0.5f) ? b : (a * (1.0f - clamped_factor)) + (b * clamped_factor);

    f32 factor0 = (clamped_factor < 0.5f) ?
        (0.5f - clamped_factor) * 2.0f :
        (clamped_factor - 0.5f) * 2.0f ;
    f32 factor1 = powf(factor0, exponent);
    f32 lerp_fac = (clamped_factor < 0.5f) ?
        0.5f - (factor1 * 0.5f) :
        0.5f + (factor1 * 0.5f) ;

    return (a * (1.0f - lerp_fac)) + (b * lerp_fac);
}



/** \brief Lerp function, basic, linear version that calculates a blend of one floating point number and another.
 *
 * \param a f16 Value to have all, none, or some of determine the output value.
 * \param b f16 Value to have all, none, or some of determine the output value.
 * \param lerp_factor f16 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \return f16 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE f16 lerpfs(const f16 a, const f16 b, const f16 lerp_factor) noexcept
{
    if (invalidfs(a) || invalidfs(b) || invalidfs(lerp_factor)) return NANfs;

    if (lerp_factor == 0.f16) return a;
    if (lerp_factor == 1.f16) return b;
    if (lerp_factor == 0.5f16) return (a + b) * 0.5f16;

    f16 lerp_fac = (lerp_factor > 1.f16) ? 1.f16 : (lerp_factor < 0.f16) ? 0.f16 : lerp_factor;

    return (a * (1.0f16 - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a f16 Value to have all, none, or some of determine the output value.
 * \param b f16 Value to have all, none, or some of determine the output value.
 * \param lerp_factor f16 Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent f16 The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerp_factor inwards towards 0.5, while values below 1.0 pull the lerping blend factor out away from 0.5.
 * \return f16 Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE f16 lerp_exp_infs(const f16 a, const f16 b, const f16 lerp_factor, const f16 exponent) noexcept
{
    if (invalidfs(a) || invalidfs(b) || invalidfs(lerp_factor) || invalidfs(exponent)) return NANfs;

    if (exponent <= 0.f16) return (a + b) * 0.5f16;

    if (lerp_factor == 0.f16) return a;
    if (lerp_factor == 1.f16) return b;
    if (lerp_factor == 0.5f16) return (a + b) * 0.5f16;

    f16 clamped_factor = (lerp_factor > 1.f16) ? 1.f16 : (lerp_factor < 0.f16) ? 0.f16 : lerp_factor;

    f16 factor0 = (clamped_factor < 0.5f16) ?
        (0.5f16 - clamped_factor) * 2.0f16 :
        (clamped_factor - 0.5f16) * 2.0f16 ;
#if __has_builtin(__builtin_powf16)
    f16 factor1 = __builtin_powf16(factor0, 1.f16 / exponent);
#else
    f16 factor1 = (f16)powf(factor0, 1.f16 / exponent);
#endif // __has_builtin
    f16 lerp_fac = (clamped_factor < 0.5f16) ?
        0.5f16 - (factor1 * 0.5f16) :
        0.5f16 + (factor1 * 0.5f16) ;
    return (a * (1.0f16 - lerp_fac)) + (b * lerp_fac);
}

/** \brief Lerp function, nonstandard version that raises the lerping blend factor by an exponent towards or away from a lerping blend value of 0.5 (half of value a and half of value b).
 *
 * \param a (const f16) Value to have all, none, or some of determine the output value.
 * \param b (const f16) Value to have all, none, or some of determine the output value.
 * \param lerp_factor (const f16) Ranging from 0.0 to 1.0, determines how much of input value b makes up the output.
 * \param exponent (const f16) The lerp_factor parameter is altered by this exponent, values above 1.0 push the lerping blend factor out away from 0.5, while values below 1.0 pull the lerp_factor inwards towards 0.5.
 * \return (f16) Output value made up of none, some, or all of the input values a and b.
 */
static HOT_INLINE f16 lerp_exp_outfs(const f16 a, const f16 b, const f16 lerp_factor, const f16 exponent) noexcept
{
    if (invalidfs(a) || invalidfs(b) || invalidfs(lerp_factor) || invalidfs(exponent)) return NANfs;

    if (lerp_factor == 0.f16) return a;
    if (lerp_factor == 1.f16) return b;
    if (lerp_factor == 0.5f16) return (a + b) * 0.5f16;

    f16 clamped_factor = (lerp_factor > 1.f16) ? 1.f16 : (lerp_factor < 0.f16) ? 0.f16 : lerp_factor;

    if (exponent <= 0.f16) return (clamped_factor < 0.5f16) ? a : (clamped_factor < 0.5f16) ? b : (a * (1.0f16 - clamped_factor)) + (b * clamped_factor);

    f16 factor0 = (clamped_factor < 0.5f16) ?
        (0.5f16 - clamped_factor) * 2.0f16 :
        (clamped_factor - 0.5f16) * 2.0f16 ;
#if __has_builtin(__builtin_powf16)
    f16 factor1 = __builtin_powf16(factor0, exponent);
#else
    f16 factor1 = (f16)powf((f32)factor0, (f32)exponent);
#endif // __has_builtin
    f16 lerp_fac = (clamped_factor < 0.5f16) ?
        0.5f16 - (factor1 * 0.5f16) :
        0.5f16 + (factor1 * 0.5f16) ;

    return (a * (1.0f16 - lerp_fac)) + (b * lerp_fac);
}
