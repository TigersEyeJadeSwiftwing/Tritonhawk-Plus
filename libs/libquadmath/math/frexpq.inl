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

/** \brief Decomposes x into its normalized fraction and exponent.
 *
 * \param x f128 The input value to decompose.
 * \param eptr s64* Pointer to store the exponent.
 * \return f128 The normalized fraction in the range [0.5, 1.0).
 */
static HOT_INLINE f128 frexpq(f128 x, s64* eptr) noexcept
{
    if (invalidq(x))
    {
        if (eptr) *eptr = 0; // Set exponent to 0 for NaN or Inf
        return x; // Preserve NaN and ±Inf
    }

    if (x == 0.0q) {
        if (eptr) *eptr = 0; // Set exponent to 0 for zero
        return 0.0q; // The fraction of zero is zero
    }

    // Normalize x to the range [0.5, 1.0)
    s64 exponent = 0;

    // Handle negative values
    if (x < 0.0q)
        x = -x; // Work with the absolute value

    // Find the exponent and normalize x
    while (x > 2.0q) { // Change to 2.0q for larger steps
        x *= 0.25q; // Divide by 4
        exponent += 2; // Increment exponent by 2
    }
    while (x >= 1.0q) {
        x *= 0.5q; // Divide by 2
        exponent += 1; // Increment exponent by 1
    }
    while (x < 0.25q) { // Change to 0.25q for larger steps
        x *= 4.0q; // Multiply by 4
        exponent -= 2; // Decrement exponent by 2
    }
    while (x < 0.5q) {
        x *= 2.0q; // Multiply by 2
        exponent -= 1; // Decrement exponent by 1
    }

    // Store the exponent
    if (eptr)
        *eptr = exponent;

    return x; // Return the normalized fraction
}
