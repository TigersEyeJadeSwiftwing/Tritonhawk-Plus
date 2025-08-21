#pragma once

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #include "roundq.inl"
    #include "powq.inl"
#endif

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

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
/** \brief Rounds a floating point number to a nearest specified power of ten.
 *
 * \param x __float128 The input value, can be any real floating-point number.
 * \param digits __float128 Controls the power of ten to round to.
 * \return __float128 The output value, rounded.
 *
 * Returns a floating-point number that is rounded to the nearest specified power of ten, or
 * "number of digits".  For example, if the "digits" parameter is 2.0, then the input "x"
 * parameter is rounded to the nearest 100.0, so an input value of 12345.064 becomes 12300.0.
 * This function also accepts negative input values for the "digits" parameter, so an input "x"
 * of 12345.064 with a value of "-2.0" for "digits" will round the 12345.064 to the nearest
 * 0.01, or nearest hundredth, resulting in an output of 12345.06.
 */
static HOT_INLINE __float128 rounddgq(__float128 x, __float128 digits)
{
    __float128 multiplier = powq(10.q, digits);
    return roundq(x / multiplier) * multiplier;
}
#else
/** \brief Rounds a floating point number to a nearest specified power of ten.
 *
 * \param x long double The input value, can be any real floating-point number.
 * \param digits long double Controls the power of ten to round to.
 * \return long double The output value, rounded.
 *
 * long double version.
 */
static HOT_INLINE long double rounddgq(long double x, long double digits)
{
    long double multiplier = powl(10.L, digits);
    return roundl(x / multiplier) * multiplier;
}
#endif // THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT

/** \brief Rounds a floating point number to a nearest specified power of ten.
 *
 * \param x double The input value, can be any real floating-point number.
 * \param digits double Controls the power of ten to round to.
 * \return double The output value, rounded.
 *
 * double version.
 */
static HOT_INLINE double rounddg(double x, double digits)
{
    double multiplier = pow(10.0, digits);
    return round(x / multiplier) * multiplier;
}

/** \brief Rounds a floating point number to a nearest specified power of ten.
 *
 * \param x float The input value, can be any real floating-point number.
 * \param digits float Controls the power of ten to round to.
 * \return float The output value, rounded.
 *
 * float version.
 */
static HOT_INLINE float rounddgf(float x, float digits)
{
    float multiplier = powf(10.f, digits);
    return roundf(x / multiplier) * multiplier;
}
