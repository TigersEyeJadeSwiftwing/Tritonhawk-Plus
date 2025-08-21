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

#if __has_builtin(__builtin_isinff128)
    /** \brief Determines if a given floating-point number is a positive or negative infinity.
     *
     * \param x (__float128) The number to check for being infinity, positive or negative.
     * \return (int) Returns 0 if false, 1 if the input is positive or negative infinity.
     */
    static HOT_INLINE int isinfq (__float128 x)
    {
        return __builtin_isinff128(x);
    }
#elif __has_builtin(__builtin_isinfq)
    /** \brief Determines if a given floating-point number is a positive or negative infinity.
     *
     * \param x (__float128) The number to check for being infinity, positive or negative.
     * \return (int) Returns 0 if false, 1 if the input is positive or negative infinity.
     */
    static HOT_INLINE int isinfq (__float128 x)
    {
        return __builtin_isinfq(x);
    }
#elif __has_builtin(__builtin_isinf)
    /** \brief Determines if a given floating-point number is a positive or negative infinity.
     *
     * \param x (__float128) The number to check for being infinity, positive or negative.
     * \return (int) Returns 0 if false, 1 if the input is positive or negative infinity.
     */
    static HOT_INLINE int isinfq (__float128 x)
    {
        return __builtin_isinf(x);
    }
#else

/** \brief Determines if a given floating-point number is a positive or negative infinity.
 *
 * \param x (__float128) The number to check for being infinity, positive or negative.
 * \return (int) Returns 0 if false, 1 if the input is positive or negative infinity.
 */
static HOT_INLINE int isinfq (__float128 x)
{
    return __builtin_isinf(x);
}

#endif
