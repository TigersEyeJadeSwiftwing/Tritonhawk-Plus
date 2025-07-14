#pragma once

#include "truncq.inl"

/*
This file is written by Tiger's Eye Jade Swiftwing, for part of a custom quadmath 128-bit floating type math library.
Note that my first name is "Tiger's Eye" (which is two words), my middle name is "Jade", and "Swiftwing" is one word that is my last name.
It is released under a zlib license, so feel free to use it, modify it, whatever, under the terms of the zlib license.

Copyright (c) 2025 Tiger's Eye Jade Swiftwing

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

/** \brief Returns an integer, from the 128-bit floating input value, truncated.
 *
 * \param a (__float128) The input value to be made into an int.
 * \return (int) The input value, converted into an int.
 *
 * Returns an integer, from the 128-bit floating input value, truncated.
 */
static inline __attribute__((always_inline, hot))
int to_intq (__float128 a)
{
    return int(truncq(a) + 0.1Q);
}

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
