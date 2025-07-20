#pragma once

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #include "truncq.inl"
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

/** \brief Returns an integer, from the 128-bit floating input value, truncated.
 *
 * \param a (__float128) The input value to be made into an int.
 * \return (int) The input value, converted into an int.
 *
 * Returns an integer, from the 128-bit floating input value, truncated.
 */
#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
static inline __attribute__((always_inline, hot))
int to_intq (__float128 a)
{
    return int(truncq(a) + 0.1Q);
}
#else
static inline __attribute__((always_inline, hot))
int to_intq (long double a)
{
    return int(truncl(a) + 0.1L);
}
#endif

static inline __attribute__((always_inline, hot))
int to_intf (float a)
{
    return int(truncf(a) + 0.1f);
}

static inline __attribute__((always_inline, hot))
int to_intd (double a)
{
    return int(trunc(a) + 0.1);
}
