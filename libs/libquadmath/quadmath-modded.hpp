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

    /* Macros */
    #define FLT128_MAX 1.18973149535723176508575932662800702e4932q
    #define FLT128_MIN 3.36210314311209350626267781732175260e-4932q
    #define FLT128_EPSILON 1.92592994438723585305597794258492732e-34q
    #define FLT128_DENORM_MIN 6.475175119438025110924438958227646552e-4966q
    #define FLT128_MANT_DIG 113
    #define FLT128_MIN_EXP (-16381)
    #define FLT128_MAX_EXP 16384
    #define FLT128_DIG 33
    #define FLT128_MIN_10_EXP (-4931)
    #define FLT128_MAX_10_EXP 4932

    #if __has_builtin(__builtin_huge_valq)
        #define HUGE_VALQ __builtin_huge_valq()
    #else
        #define HUGE_VALQ (__extension__ 0x1.0p32767q)
    #endif

    using namespace TritonhawkPlus;

    inline static constexpr f128 M_Eq =		        2.718281828459045235360287471352662498q;  /* e */
    inline static constexpr f128 M_LOG2Eq =	        1.442695040888963407359924681001892137q;  /* log_2 e */
    inline static constexpr f128 M_LOG10Eq =	    0.434294481903251827651128918916605082q;  /* log_10 e */
    inline static constexpr f128 M_LN2q =		    0.693147180559945309417232121458176568q;  /* log_e 2 */
    inline static constexpr f128 M_LN10q =		    2.302585092994045684017991454684364208q;  /* log_e 10 */
    inline static constexpr f128 M_PIq =		    3.141592653589793238462643383279502884q;  /* pi */
    inline static constexpr f128 M_PI_2q =		    1.570796326794896619231321691639751442q;  /* pi/2 */
    inline static constexpr f128 M_PI_4q =		    0.785398163397448309615660845819875721q;  /* pi/4 */
    inline static constexpr f128 M_1_PIq =		    0.318309886183790671537767526745028724q;  /* 1/pi */
    inline static constexpr f128 M_2_PIq =		    0.636619772367581343075535053490057448q;  /* 2/pi */
    inline static constexpr f128 M_2_SQRTPIq =	    1.128379167095512573896158903121545172q;  /* 2/sqrt(pi) */
    inline static constexpr f128 M_SQRT2q =	        1.414213562373095048801688724209698079q;  /* sqrt(2) */
    inline static constexpr f128 M_SQRT1_2q =	    0.707106781186547524400844362104849039q;  /* 1/sqrt(2) */

    inline static constexpr f128 INFINITYq =        (f128)INFINITY;
    inline static constexpr f128 INFINITYnq =       -(f128)INFINITY;
    inline static constexpr f128 NANq =             (f128)NAN;

    inline static constexpr f32 INFINITYf =         (f32)INFINITY;
    inline static constexpr f32 INFINITYnf =        -(f32)INFINITY;
    inline static constexpr f32 NANf =              (f32)NAN;

    inline static constexpr f16 INFINITYfs =        (f16)INFINITY;
    inline static constexpr f16 INFINITYnfs =       -(f16)INFINITY;
    inline static constexpr f16 NANfs =             (f16)NAN;

    // Custom, THP-specific
    inline static constexpr f128 M_SQRT2_m1q = M_SQRT2q - 1.q;
    inline static constexpr f128 SMP_GRID_ELLIPSE_COS_FACTOR = M_PI_2q / M_SQRT2_m1q;

    #ifdef __cplusplus
    }
    #endif

    using namespace TritonhawkPlus;

    #include "isnanq.inl"
    #include "isinfq.inl"
    #include "invalidq.inl"

    // two-term split of 2/π = 0.636619772367581343075535053490057448137838582975…
    inline static constexpr __float128 TWO_OVER_PI_HI
      = 0x1.28be60db9391054a84a8ff7988eaadbbp-1q;
    inline static constexpr __float128 TWO_OVER_PI_LO
      = 0x1.60608c13e6c574ff434c37d862e28e2p-114q;

    // three-term split of π/2 = 1.570796326794896619231321691639751442098584699687…
    inline static constexpr __float128 PI_OVER_2_HI
      = 0x1.921fb54442d18469898cc517afb44a409p+0q;
    inline static constexpr __float128 PI_OVER_2_MI
      = 0x1.d8ef9c67fbb45796cce8c97fa4fcdc13p-112q;
    inline static constexpr __float128 PI_OVER_2_LO
      = 0x1.736cc9801a5bc01bbc775ed214f44bea8p-225q;

    #include "fmaq.inl"

    //------------------------------------------------------------------------------
    // Polynomial kernels on |r| ≤ π/4
    //------------------------------------------------------------------------------
    static HOT_INLINE __float128 cos_kernel(const __float128 r) noexcept
    {
        static constexpr __float128 C[7] =
        {
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
        for (s8 i = 5; i >= 0; --i)
            p = fmaq(p, r2, C[i]);

        return fmaq(p, r2, 1.0q);
    }

    static HOT_INLINE __float128 sin_kernel(const __float128 r) noexcept
    {
        static constexpr __float128 S[7] =
        {
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
        for (s8 i = 5; i >= 0; --i)
            p = fmaq(p, r2, S[i]);

        return fmaq(p, r2, 1.0q) * r;
    }

    #ifndef __has_builtin
        #define __has_builtin(x) 0 // Compatibility with non-Clang compilers
    #endif

    #include "quadmath-imp.hpp"

    #include "ceilq.inl"
    #include "copysignq.inl"
    #include "cosq.inl"
    #include "expq.inl"
    #include "fabsq.inl"

    #include "floorq.inl"
    #include "fmaq.inl"
    #include "fmaxq.inl"
    #include "fminq.inl"
    #include "fmodq.inl"

    #include "frexpq.inl"
    #include "ldexpq.inl"
    #include "lnq.inl"
    #include "modfq.inl"
    #include "nearbyintq.inl"

    #include "powq.inl"
    #include "roundq.inl"
    #include "scalbnq.inl"
    #include "sinq.inl"
    #include "sqrtq.inl"

    #include "tanq.inl"
    #include "truncq.inl"

    static HOT_INLINE f16 ceilfs(const f16 x) noexcept { return (f16) ceil(f64(x)); }
    static HOT_INLINE f16 copysignfs(const f16 x, const f16 y) noexcept { return (f16) copysignf(f32(x), f32(y)); }
    static HOT_INLINE f16 cosfs(const f16 x) noexcept { return (f16) cos(f64(x)); }
    static HOT_INLINE f16 expfs(const f16 x) noexcept { return (f16) exp(f64(x)); }
    static HOT_INLINE f16 fabsfs(const f16 x) noexcept { return (f16) fabsf(f32(x)); }

    static HOT_INLINE f16 floorfs(const f16 x) noexcept { return (f16) floor(f64(x)); }
    static HOT_INLINE f16 fmafs(const f16 a, const f16 b, const f16 c) noexcept { return (f16) fma(f64(a), f64(b), f64(c)); }
    static HOT_INLINE f16 fmaxfs(const f16 a, const f16 b) noexcept { return (f16) std::fmaxf(f32(a), f32(b)); }
    static HOT_INLINE f16 fminfs(const f16 a, const f16 b) noexcept { return (f16) std::fminf(f32(a), f32(b)); }
    static HOT_INLINE f16 fmodfs(const f16 x, const f16 y) noexcept { return (f16) fmod(f64(x), f64(y)); }

    static HOT_INLINE f16 frexpfs(const f16 x, s64* eptr) noexcept { return (f16) frexp(f64(x), (int*)eptr); }
    static HOT_INLINE f16 ldexpfs(const f16 a, const s64 exp) noexcept { return (f16) ldexp(f64(a), exp); }
    static HOT_INLINE f16 logfs(const f16 x) noexcept { return (f16) log(f64(x)); }
    static HOT_INLINE f16 modffs(const f16 x, f16* iptr) noexcept { return (f16) modf(f64(x), (f64*)iptr); }
    static HOT_INLINE f16 nearbyintfs(const f16 x) noexcept { return (f16) nearbyint(f64(x)); }

    static HOT_INLINE f16 powfs(const f16 x, const f16 y) noexcept { return (f16) pow(f64(x), f64(y)); }
    static HOT_INLINE f16 roundfs(const f16 x) noexcept { return (f16) roundf(f32(x)); }
    static HOT_INLINE f16 scalbnfs(const f16 a, const s64 exp) noexcept { return (f16) scalbn(f64(a), exp); }
    static HOT_INLINE f16 sinfs(const f16 x) noexcept { return (f16) sin(f64(x)); }
    static HOT_INLINE f16 sqrtfs(const f16 x) noexcept { return (f16) sqrt(f64(x)); }

    #include "clampq.inl"
    #include "distwarpq.inl"
    #include "issignalingq.inl"
    #include "lerp.inl"
    #include "rounddgq.inl"
    #include "to_intq.inl"

    /*
    #if __has_identifier(f16)
        inline constexpr auto &fs = f16;
    #else
        static HOT_INLINE f16 fs(const f16 x) noexcept { return (f16) f(f32(x)); }
    #endif

    #if __has_builtin(__builtin_)
        inline constexpr auto &fs = __builtin_f16;
    #else
        static HOT_INLINE f16 fs(const f16 x) noexcept { return (f16) f(f32(x)); }
    #endif
    */
};

#endif
