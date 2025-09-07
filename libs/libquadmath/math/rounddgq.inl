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

#include "roundq.inl"
#include "powq.inl"

/** \brief Rounds a floating point number to a nearest specified power of ten.
 *
 * \param x (const f128) The input value, can be any real floating-point number.
 * \param digits (const s8) Controls the power of ten to round to.
 * \return (f128) The output value, rounded.
 *
 * Returns a floating-point number that is rounded to the nearest specified power of ten, or
 * "number of digits".  The name "rounddg" is from "round digits", and, similar to other math
 * library functions, has versions for different data types, such as having a "q" for
 * "quad precision", which is for 128-bit precision, or additional letter for 64-bit data types
 * such as a "double", or an "f" for handling 32-bit floating-point data types such as "float".
 *
 * This function is like a standard rounding function that rounds a floating-point variable to a
 * whole number that is floating-point, with the same number of bits, except that this function
 * rounds the variable to a nearest multiple of ten, or specific digits to the left or right of
 * the decimal point, specified by the second input parameter which is an exponent of ten.
 *
 * For example, if the "digits" parameter is 2, then the input "x" parameter is rounded to the
 * nearest 100.0q, so an input value of 12345.064q becomes 12300.0q.
 *
 * This function also accepts negative input values for the "digits" parameter, so an input "x"
 * of 12345.064q with a value of "-2" for "digits" will round the 12345.064q to the nearest
 * 0.01q (the nearest hundredth), resulting in an output of 12345.06q.
 */
static HOT_INLINE f128 rounddgq(const f128 x, const s8 digits) noexcept
{
    if (invalidq(x)) return NANq;
    if (digits == 0) return roundq(x);

    constexpr f128 POW10_POS[] =
    {
        1e0q,  1e1q,  1e2q,  1e3q,  1e4q,  1e5q,  1e6q,  1e7q,  1e8q,  1e9q,
        1e10q, 1e11q, 1e12q, 1e13q, 1e14q, 1e15q, 1e16q, 1e17q, 1e18q, 1e19q,
        1e20q, 1e21q, 1e22q, 1e23q, 1e24q, 1e25q, 1e26q, 1e27q, 1e28q, 1e29q,
        1e30q, 1e31q, 1e32q, 1e33q, 1e34q, 1e35q, 1e36q, 1e37q
    };
    constexpr f128 POW10_NEG[] =
    {
        1e0q,   1e-1q,  1e-2q,  1e-3q,  1e-4q,  1e-5q,  1e-6q,  1e-7q,  1e-8q,  1e-9q,
        1e-10q, 1e-11q, 1e-12q, 1e-13q, 1e-14q, 1e-15q, 1e-16q, 1e-17q, 1e-18q, 1e-19q,
        1e-20q, 1e-21q, 1e-22q, 1e-23q, 1e-24q, 1e-25q, 1e-26q, 1e-27q, 1e-28q, 1e-29q,
        1e-30q, 1e-31q, 1e-32q, 1e-33q, 1e-34q, 1e-35q, 1e-36q, 1e-37q
    };
    constexpr s8 POW10_MAX = 38;
    constexpr s8 POW10_MIN = -38;

    f128 multiplier = 1.q, divisor = 1.q;

    if ((digits > POW10_MAX) || (digits < POW10_MIN))
    {
        multiplier = powq(10.q, f128(digits));
        divisor = powq(10.q, f128(-digits));
    }
    else if (digits > 0)
    {
        multiplier = POW10_POS[digits];
        divisor = POW10_NEG[digits];
    }
    else if (digits < 0)
    {
        multiplier = POW10_NEG[-digits];
        divisor = POW10_POS[-digits];
    }

    if (invalidq(multiplier) || invalidq(divisor)) return NANq;

    f128 result = roundq(x * divisor) * multiplier;
    if (invalidq(result)) return NANq;

    return result;
}

/** \brief Rounds a floating point number to a nearest specified power of ten.
 *
 * \param x (const f64) The input value, can be any real floating-point number.
 * \param digits (const s8) Controls the power of ten to round to.
 * \return (f64) The output value, rounded.
 *
 * 64-bit version.  Promotes input values to 128-bits for precision, before converting back to 64-bits.
 */
static HOT_INLINE f64 rounddg(const f64 x, const s8 digits) noexcept
{
    if (invalid(x)) return NAN;
    return f64 (rounddgq(f128 (x), digits));
}

/** \brief Rounds a floating point number to a nearest specified power of ten.
 *
 * \param x (const f32) The input value, can be any real floating-point number.
 * \param digits (const s8) Controls the power of ten to round to.
 * \return (f32) The output value, rounded.
 *
 * 32-bit version.  Promotes input values to 128-bits for precision, before converting back to 32-bits.
 */
static HOT_INLINE f32 rounddgf(const f32 x, const s8 digits) noexcept
{
    if (invalidf(x)) return NAN;
    return f32 (rounddgq(f128 (x), digits));
}

/** \brief Rounds a floating point number to a nearest specified power of ten.
 *
 * \param x (const f16) The input value, can be any real floating-point number.
 * \param digits (const s8) Controls the power of ten to round to.
 * \return (f16) The output value, rounded.
 *
 * 16-bit version.  Promotes input values to 128-bits for precision, before converting back to 16-bits.
 */
static HOT_INLINE f16 rounddgfs(const f16 x, const s8 digits) noexcept
{
    if (invalidfs(x)) return NAN;
    return f16 (rounddgq(f128 (x), digits));
}
