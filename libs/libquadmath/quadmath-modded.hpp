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

/*
Note that this file has been heavily modified from the original, and is not much like
the libquadmath library it was derived from.  Many of the inline files this version
includes are licensed under the GPL version 3 license.
*/

#ifndef QUADMATH_H
#define QUADMATH_H

#include <stdlib.h>

#include <cstdint>
#include <limits>

namespace quadmath
{
    #ifdef __cplusplus
    extern "C" {
    #endif

    /* Define the complex type corresponding to __float128
       ("_Complex __float128" is not allowed) */
    /*
    #if (!defined(_ARCH_PPC)) || defined(__LONG_DOUBLE_IEEE128__)
    typedef _Complex float __attribute__((mode(TC))) __complex128;
    #else
    typedef _Complex float __attribute__((mode(KC))) __complex128;
    #endif
    */

    /*
    #ifdef __cplusplus
    # define __quadmath_throw throw ()
    # define __quadmath_nth(fct) fct throw ()
    #else
    # define __quadmath_throw __attribute__((__nothrow__))
    # define __quadmath_nth(fct) __attribute__((__nothrow__)) fct
    #endif
    */

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

    inline static constexpr __float128 M_Eq =		    2.718281828459045235360287471352662498q;  /* e */
    inline static constexpr __float128 M_LOG2Eq =	    1.442695040888963407359924681001892137q;  /* log_2 e */
    inline static constexpr __float128 M_LOG10Eq =	    0.434294481903251827651128918916605082q;  /* log_10 e */
    inline static constexpr __float128 M_LN2q =		    0.693147180559945309417232121458176568q;  /* log_e 2 */
    inline static constexpr __float128 M_LN10q =		2.302585092994045684017991454684364208q;  /* log_e 10 */
    inline static constexpr __float128 M_PIq =		    3.141592653589793238462643383279502884q;  /* pi */
    inline static constexpr __float128 M_PI_2q =		1.570796326794896619231321691639751442q;  /* pi/2 */
    inline static constexpr __float128 M_PI_4q =		0.785398163397448309615660845819875721q;  /* pi/4 */
    inline static constexpr __float128 M_1_PIq =		0.318309886183790671537767526745028724q;  /* 1/pi */
    inline static constexpr __float128 M_2_PIq =		0.636619772367581343075535053490057448q;  /* 2/pi */
    inline static constexpr __float128 M_2_SQRTPIq =	1.128379167095512573896158903121545172q;  /* 2/sqrt(pi) */
    inline static constexpr __float128 M_SQRT2q =	    1.414213562373095048801688724209698079q;  /* sqrt(2) */
    inline static constexpr __float128 M_SQRT1_2q =	    0.707106781186547524400844362104849039q;  /* 1/sqrt(2) */

    inline static constexpr __float128 M_SQRT2_m1q = M_SQRT2q - 1.q;
    inline static constexpr __float128 SMP_GRID_ELLIPSE_COS_FACTOR = M_PI_2q / M_SQRT2_m1q;
    inline static constexpr __float128 INFINITYq =      (__float128)INFINITY;
    inline static constexpr __float128 INFINITYnq =     -(__float128)INFINITY;
    inline static constexpr __float128 NANq =           (__float128)NAN;

    #ifdef __cplusplus
    }
    #endif

    //------------------------------------------------------------------------------
    // Bit-pattern → __float128 initializer
    //------------------------------------------------------------------------------
    union Bits128
    {
        __float128 f;
        struct { uint64_t lo, hi; };
    };

    static inline __float128 make_f128(uint64_t hi, uint64_t lo) {
        /*
        Bits128 u;
        u.hi = hi;
        u.lo = lo;
        return u.f;
        */

        union
        {
            unsigned __int128 i;
            __float128       f;
        } u;
        u.i = ( (unsigned __int128)hi << 64 ) | lo;
        return u.f;
    }

    /*
    static const __float128 TWO_OVER_PI_HI = make_f128(0x28be60db9391054aULL, 0x84a8ff7988eaadbbULL);
    static const __float128 TWO_OVER_PI_LO = make_f128(0xfb8a7e8f1e2863f2ULL, 0xd59e51c89366e5f8ULL);

    static const __float128 PI_OVER_2_HI = make_f128(0xc90fdaa22168c234ULL, 0xc4c6628b80dc1cd1ULL);
    static const __float128 PI_OVER_2_MI = make_f128(0x29024e088a67cc74ULL, 0xa918fdc8ba30fbc2ULL);
    static const __float128 PI_OVER_2_LO = make_f128(0x86e220b9f33d1d1eULL, 0xdfe3c84680b74a13ULL);
    */

    // TWO_OVER_PI_HI, TWO_OVER_PI_LO, PI_OVER_2_HI, PI_OVER_2_MI, PI_OVER_2_LO

    // two-term split of 2/π = 0.636619772367581343075535053490057448137838582975…
    static constexpr __float128 TWO_OVER_PI_HI
      = 0x1.28be60db9391054a84a8ff7988eaadbbp-1q;
    static constexpr __float128 TWO_OVER_PI_LO
      = 0x1.60608c13e6c574ff434c37d862e28e2p-114q;

    // three-term split of π/2 = 1.570796326794896619231321691639751442098584699687…
    static constexpr __float128 PI_OVER_2_HI
      = 0x1.921fb54442d18469898cc517afb44a409p+0q;
    static constexpr __float128 PI_OVER_2_MI
      = 0x1.d8ef9c67fbb45796cce8c97fa4fcdc13p-112q;
    static constexpr __float128 PI_OVER_2_LO
      = 0x1.736cc9801a5bc01bbc775ed214f44bea8p-225q;

    #include "fmaq.inl"

    //------------------------------------------------------------------------------
    // Polynomial kernels on |r| ≤ π/4
    //------------------------------------------------------------------------------
    static HOT_INLINE __float128 cos_kernel(__float128 r) {
        static constexpr __float128 C[7] = {
            -0.5q,
             0.041666666666666666666666666666666666q,
            -0.001388888888888888888888888888888889q,
             0.000024801587301587301587301587301587q,
            -0.00000027557319223985890652557319224q,
             0.00000000208767569878680989792100903q,
            -0.00000000001147074559772972635174139q
        };
        __float128 r2 = r*r;
        __float128 p = C[6];
        for (int i = 5; i >= 0; --i)
            p = fmaq(p, r2, C[i]);
        return fmaq(p, r2, 1.0q);
    }

    static HOT_INLINE __float128 sin_kernel(__float128 r) {
        static constexpr __float128 S[7] = {
            -0.166666666666666666666666666666666667q,
             0.008333333333333333333333333333333333q,
            -0.0001984126984126984126984126984126984127q,
             0.000002755731922398589065255731922398589q,
            -0.0000000250521083854417187750521083854417188q,
             0.0000000001605904383682161451624886738535072q,
            -0.000000000000764716373181981717957666248517q
        };
        __float128 r2 = r*r;
        __float128 p = S[6];
        for (int i = 5; i >= 0; --i)
            p = fmaq(p, r2, S[i]);
        return fmaq(p, r2, 1.0q) * r;
    }

    #ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
        #include "quadmath-imp.hpp"

        /*
        #if defined(LIBM_SCCS) && !defined(lint)
        static char rcsid[] = "$NetBSD: $";
        #endif
        */

        #ifndef __has_builtin
            #define __has_builtin(x) 0 // Compatibility with non-Clang compilers
        #endif

        using namespace TritonhawkPlus;

        #include "copysignq.inl"
        #include "sinq.inl"
        #include "cosq.inl"
        #include "tanq.inl"
        #include "expq.inl"
        #include "fabsq.inl"
        #include "lnq.inl"
        #include "powq.inl"
        #include "sqrtq.inl"
        #include "isnanq.inl"
        #include "isinfq.inl"
        #include "ldexpq.inl"
        #include "frexpq.inl"
        #include "scalbnq.inl"
        #include "fminq.inl"
        #include "fmaxq.inl"
        #include "floorq.inl"
        #include "ceilq.inl"
        #include "roundq.inl"
        #include "truncq.inl"
        #include "fmodq.inl"
        #include "modfq.inl"
        #include "nearbyintq.inl"
        #include "fmaq.inl"

        #include "issignalingq.inl"

        #include "rounddgq.inl"
        #include "lerp.inl"
        #include "clampq.inl"
        #include "distwarpq.inl"
        #include "to_intq.inl"
    #else
        #include "quadmath-imp.hpp"

        /*
        inline constexpr auto &ceilq = ceill;
        inline constexpr auto &copysignq = copysignl;
        inline constexpr auto &expq = expl;
        inline constexpr auto &fabsq = fabsl;
        inline constexpr auto &floorq = floorl;
        inline constexpr auto &fmaxq = fmaxl;
        inline constexpr auto &fminq = fminl;
        inline constexpr auto &fmodq = fmodl;
        inline constexpr auto &frexpq = frexpl;
        // inline constexpr auto &isinfq = isinf;
        // inline constexpr auto &isnanq = isnan;
        // inline constexpr auto &issignalingq = issignalingl;
        inline constexpr auto &lnq = logl;
        inline constexpr auto &powq = powl;
        inline constexpr auto &roundq = roundl;
        inline constexpr auto &scalbnq = scalbnl;
        inline constexpr auto &sqrtq = sqrtl;
        inline constexpr auto &truncq = truncl;
        inline constexpr auto &sinq = sinl;
        inline constexpr auto &cosq = cosl;
        inline constexpr auto &tanq = tanl;
        */

        using namespace TritonhawkPlus;

        HOT_INLINE static f128 ceilq(f128 x) noexcept { return std::ceill(x); };
        HOT_INLINE static f128 copysignq(f128 a, f128 b) noexcept { return std::copysignl(a, b); };
        HOT_INLINE static f128 expq(f128 x) noexcept { return std::expl(x); };
        HOT_INLINE static f128 fabsq(f128 x) noexcept { return std::fabsl(x); };
        HOT_INLINE static f128 floorq(f128 x) noexcept { return std::floorl(x); };
        // HOT_INLINE static f128 fmaxq(f128 a, f128 b) noexcept { return std::fmaxl(a, b); };
        // HOT_INLINE static f128 fminq(f128 a, f128 b) noexcept { return std::fminl(a, b); };
        // HOT_INLINE static f128 fmodq(f128 x, f128 y) noexcept { return std::fmodl(x, y); };
        HOT_INLINE static f128 frexpq(f128 x, int* eptr) noexcept { return std::frexpl(x, eptr); };
        HOT_INLINE static bool isinfq(f128 x) noexcept { return std::isinf(x); };
        HOT_INLINE static bool isnanq(f128 x) noexcept { return std::isnan(x); };
        HOT_INLINE static f128 lnq(f128 x) noexcept { return std::logl(x); };
        HOT_INLINE static f128 logq(f128 x) noexcept { return std::logl(x); };
        // HOT_INLINE static f128 powq(f128 x, f128 y) noexcept { return std::powl(x, y); };
        // HOT_INLINE static f128 roundq(f128 x) noexcept { return std::roundl(x); };
        HOT_INLINE static f128 scalbnq(f128 x, int exp) noexcept { return std::scalbnl(x, exp); };
        // HOT_INLINE static f128 sqrtq(f128 x) noexcept { return std::sqrtl(x); };
        // HOT_INLINE static f128 truncq(f128 x) noexcept { return std::truncl(x); };
        // HOT_INLINE static f128 sinq(f128 x) noexcept { return std::sinl(x); };
        // HOT_INLINE static f128 cosq(f128 x) noexcept { return std::cosl(x); };
        // HOT_INLINE static f128 tanq(f128 x) noexcept { return std::tanl(x); };
        HOT_INLINE static f128 modfq(f128 num, f128* iptr) noexcept { return std::modfl(num, iptr); };
        HOT_INLINE static f128 ldexpq(f128 x, int exp) noexcept { return std::ldexpl(x, exp); };
        HOT_INLINE static f128 nearbyintq(f128 x) noexcept { return std::nearbyintl(x); };

        #include "fminq.inl"
        #include "fmaxq.inl"
        #include "powq.inl"
        #include "sqrtq.inl"
        #include "roundq.inl"
        #include "truncq.inl"
        #include "fmodq.inl"
        #include "sinq.inl"
        #include "cosq.inl"
        #include "tanq.inl"

        #include "clampq.inl"
        #include "distwarpq.inl"
        #include "lerp.inl"
        #include "to_intq.inl"
        #include "rounddgq.inl"
    #endif
};

#endif
