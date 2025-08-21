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

#include <cstdint> // For int64_t and uint64_t
#include <limits> // For numeric_limits

// A simple high-precision __float128 FMA function
/*
I'm still getting a crash when calling cosq(0.1q);  Here are the functions as they are now:
static HOT_INLINE __float128 fmaq(__float128 a, __float128 b, __float128 c) {
    // A constant used for splitting floats, typically 2^64 + 1 for __float128
    static constexpr __float128 SPLIT = 0x1p64q + 1.0q;

    // --- Step 1: High-precision multiplication (a * b) using Dekker's algorithm ---
    // Correctly split a into two parts: ah (high) and al (low)
    __float128 temp_a = a * SPLIT;
    __float128 ah = a + temp_a - temp_a;
    __float128 al = a - ah;

    // Correctly split b into two parts: bh (high) and bl (low)
    __float128 temp_b = b * SPLIT;
    __float128 bh = b + temp_b - temp_b;
    __float128 bl = b - bh;

    // Compute the product in two parts:
    // p1 = a * b (the rounded, normal product)
    // p2 = a * b - p1 (the error term)
    __float128 p1 = a * b;
    __float128 p2 = ((ah * bh - p1) + ah * bl + al * bh) + al * bl;

    // --- Step 2: High-precision addition of (p1, p2) + c ---
    // The sum of p1 + c is performed first.
    __float128 sum = p1 + c;

    // The result is the sum plus the "leftover" error terms,
    // which are added in a way to preserve precision.
    __float128 error = p1 - sum + c;
    __float128 result = sum + p2 + error;

    return result;
}
*/

/**
 * \brief Computes a*b + c with one rounding error, using a robust TwoProd and TwoSum algorithm.
 *
 * \param a First multiplicand.
 * \param b Second multiplicand.
 * \param c Addend.
 * \return __float128 The result of the fused multiply-add operation.
 */
/*
static HOT_INLINE __float128 fmaq(__float128 a, __float128 b, __float128 c) {
    // --- Step 1: High-precision multiplication (a * b) using Dekker's algorithm ---
    // A constant used for splitting floats, typically 2^64 + 1 for __float128
    static constexpr __float128 SPLIT = 0x1.0p64q + 1.0q;

    __float128 temp_a = a * SPLIT;
    __float128 ah = temp_a - (temp_a - a);
    __float128 al = a - ah;

    __float128 temp_b = b * SPLIT;
    __float128 bh = temp_b - (temp_b - b);
    __float128 bl = b - bh;

    // The product a*b, which is the high-order part of the result
    __float128 p1 = a * b;

    // The error term of the product
    __float128 p2 = ((ah * bh - p1) + ah * bl + al * bh) + al * bl;

    // --- Step 2: High-precision addition of (p1, p2) + c using the TwoSum algorithm ---
    // The sum of p1 + c is performed first
    __float128 sum = p1 + c;

    // The error term of the addition
    __float128 error = p1 - sum + c;

    // The final result is the sum of the rounded result and the two error terms
    __float128 result = sum + (p2 + error);

    return result;
}
*/

/**
 * \brief Computes a*b + c with one rounding error, using a robust TwoProd and TwoSum algorithm.
 *
 * \param a First multiplicand.
 * \param b Second multiplicand.
 * \param c Addend.
 * \return __float128 The result of the fused multiply-add operation.
 */
static HOT_INLINE __float128 fmaq(__float128 a, __float128 b, __float128 c) {
    // --- TwoProd(a, b) ---
    static constexpr __float128 SPLIT = 0x1.0p57q + 1.0q;
    __float128 ap = a * SPLIT, ah = ap - (ap - a), al = a - ah;
    __float128 bp = b * SPLIT, bh = bp - (bp - b), bl = b - bh;
    __float128 p1 = a * b;
    __float128 p2 = ((ah*bh - p1) + ah*bl + al*bh) + al*bl;

    // --- TwoSum(p1, c) ---
    __float128 s     = p1 + c;
    __float128 err   = (p1 - s) + c;

    return s + (p2 + err);
}
