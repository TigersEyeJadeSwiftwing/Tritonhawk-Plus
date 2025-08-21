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

#if __has_builtin(__builtin_fabsf128)
    /** \brief Returns the absolute value of a 128-bit floating point value.
     *
     * \param x __float128 The input value to get the absolute value of.
     * \return __float128 The input value, but always positive (zero or greater).
     */
    static HOT_INLINE __float128 fabsq(__float128 x)
    {
        return __builtin_fabsf128(x);
    }
#elif __has_builtin(__builtin_fabsq)
    /** \brief Returns the absolute value of a 128-bit floating point value.
     *
     * \param x __float128 The input value to get the absolute value of.
     * \return __float128 The input value, but always positive (zero or greater).
     */
    static HOT_INLINE __float128 fabsq(__float128 x)
    {
        return __builtin_fabsq(x);
    }
#else

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #include "isnanq.inl"
    #include "isinfq.inl"
#endif

/** \brief Returns the absolute value of a 128-bit floating point value.
 *
 * \param x __float128 The input value to get the absolute value of.
 * \return __float128 The input value, but always positive (zero or greater).
 */
static HOT_INLINE __float128 fabsq(__float128 x)
{
    if (isnanq(x)) return x;

    return x >= 0.q ? x : -x;
}

#endif
