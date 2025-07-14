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

/** \brief Returns a floating-point input value of any size that is restricted to an output range of -1.0 to 1.0.
 *
 * \param a (__float128) The input value, can be any real floating-point number.
 * \return (__float128) The output value, which is the input limited on a curve to -1.0 to 1.0.
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
static inline __float128 distwarpq (__float128 a)
{
    return a / ( ((a < 0) ? -a : a) + 1.Q );
}
