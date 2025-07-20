#pragma once

/* GCC Quad-Precision Math Library
   Copyright (C) 2010-2019 Free Software Foundation, Inc.
   Written by Francois-Xavier Coudert  <fxcoudert@gcc.gnu.org>

This file is part of the libquadmath library.
Libquadmath is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

Libquadmath is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with libquadmath; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
Boston, MA 02110-1301, USA.  */

#ifndef QUADMATH_H
#define QUADMATH_H

#include <stdlib.h>

namespace quadmath
{
    #ifdef __cplusplus
    extern "C" {
    #endif

    /* Define the complex type corresponding to __float128
       ("_Complex __float128" is not allowed) */
    #if (!defined(_ARCH_PPC)) || defined(__LONG_DOUBLE_IEEE128__)
    typedef _Complex float __attribute__((mode(TC))) __complex128;
    #else
    typedef _Complex float __attribute__((mode(KC))) __complex128;
    #endif

    #ifdef __cplusplus
    # define __quadmath_throw throw ()
    # define __quadmath_nth(fct) fct throw ()
    #else
    # define __quadmath_throw __attribute__((__nothrow__))
    # define __quadmath_nth(fct) __attribute__((__nothrow__)) fct
    #endif

    /* Macros */
    #define FLT128_MAX 1.18973149535723176508575932662800702e4932Q
    #define FLT128_MIN 3.36210314311209350626267781732175260e-4932Q
    #define FLT128_EPSILON 1.92592994438723585305597794258492732e-34Q
    #define FLT128_DENORM_MIN 6.475175119438025110924438958227646552e-4966Q
    #define FLT128_MANT_DIG 113
    #define FLT128_MIN_EXP (-16381)
    #define FLT128_MAX_EXP 16384
    #define FLT128_DIG 33
    #define FLT128_MIN_10_EXP (-4931)
    #define FLT128_MAX_10_EXP 4932

    #define HUGE_VALQ __builtin_huge_valq()
    /* The following alternative is valid, but brings the warning:
       (floating constant exceeds range of ‘__float128’)  */
    /* #define HUGE_VALQ (__extension__ 0x1.0p32767Q) */

    #define M_Eq		2.718281828459045235360287471352662498Q  /* e */
    #define M_LOG2Eq	1.442695040888963407359924681001892137Q  /* log_2 e */
    #define M_LOG10Eq	0.434294481903251827651128918916605082Q  /* log_10 e */
    #define M_LN2q		0.693147180559945309417232121458176568Q  /* log_e 2 */
    #define M_LN10q		2.302585092994045684017991454684364208Q  /* log_e 10 */
    #define M_PIq		3.141592653589793238462643383279502884Q  /* pi */
    #define M_PI_2q		1.570796326794896619231321691639751442Q  /* pi/2 */
    #define M_PI_4q		0.785398163397448309615660845819875721Q  /* pi/4 */
    #define M_1_PIq		0.318309886183790671537767526745028724Q  /* 1/pi */
    #define M_2_PIq		0.636619772367581343075535053490057448Q  /* 2/pi */
    #define M_2_SQRTPIq	1.128379167095512573896158903121545172Q  /* 2/sqrt(pi) */
    #define M_SQRT2q	1.414213562373095048801688724209698079Q  /* sqrt(2) */
    #define M_SQRT1_2q	0.707106781186547524400844362104849039Q  /* 1/sqrt(2) */

    #ifdef __cplusplus
    }
    #endif

    #ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
        #include "quadmath-imp.hpp"

        #if defined(LIBM_SCCS) && !defined(lint)
        static char rcsid[] = "$NetBSD: $";
        #endif

        #include "ceilq.inl"
        #include "clampq.inl"
        #include "copysignq.inl"
        #include "distwarpq.inl"
        #include "expq.inl"
        #include "fabsq.inl"
        #include "floorq.inl"
        #include "fmaxq.inl"
        #include "fminq.inl"
        #include "fmodq.inl"
        #include "frexpq.inl"
        #include "isinfq.inl"
        #include "isnanq.inl"
        #include "issignalingq.inl"
        #include "lerp.inl"
        #include "lnq.inl"
        #include "powq.inl"
        #include "roundq.inl"
        #include "scalbnq.inl"
        #include "sqrtq.inl"
        #include "to_intq.inl"
        #include "truncq.inl"
    #else
        inline constexpr auto &ceilq = ceill;
        inline constexpr auto &copysignq = copysignl;
        inline constexpr auto &expq = expl;
        inline constexpr auto &fabsq = fabsl;
        inline constexpr auto &floorq = floorl;
        inline constexpr auto &fmaxq = fmaxl;
        inline constexpr auto &fminq = fminl;
        inline constexpr auto &fmodq = fmodl;
        inline constexpr auto &frexpq = frexpl;
        // inline constexpr auto &isinfq = isinfl;
        // inline constexpr auto &isnanq = isnanl;
        // inline constexpr auto &issignalingq = issignalingl;
        // inline constexpr auto &lnq = lnl;
        inline constexpr auto &powq = powl;
        inline constexpr auto &roundq = roundl;
        inline constexpr auto &scalbnq = scalbnl;
        inline constexpr auto &sqrtq = sqrtl;
        inline constexpr auto &truncq = truncl;

        #include "clampq.inl"
        #include "distwarpq.inl"
        #include "lerp.inl"
        #include "to_intq.inl"
    #endif

};

#endif
