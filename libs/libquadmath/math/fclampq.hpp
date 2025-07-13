#pragma once

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

/** \brief Return a value that ranges from X to Y, including the values of both X and Y as potential output values.
 *
 * \param v (__float128) The input value, to be limited to being between the other two parameters.
 * \param x (__float128) The lower limit.  If the input value "v" is below this value, this value will be returned instead.
 * \param y (__float128) The upper limit.  If the input value "v" is above this value, this value will be returned instead.
 * \return (__float128) The input value "v", but limited to the range of the input values "x" and "y".
 *
 * Return a value that ranges from X to Y, including the values of both X and Y as potential output values.  128-bit float version.
 * This is pretty much a combination of min() and max() functions combined into one, nothing fancy here.
 * If you have trouble understanding it, and you're not brand-new to C or C-style languages, you may want to reconsider being a C/C++ programmer...
 */
static inline __float128 fclampq (__float128 v, __float128 x, __float128 y)
{
    return (v >= x) ? ((v <= y) ? v : y) : x;
}
