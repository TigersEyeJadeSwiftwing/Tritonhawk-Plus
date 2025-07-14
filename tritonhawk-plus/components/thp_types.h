#pragma once

#include "config_h/clang64/config.h"

#ifndef GIMP_DISABLE_COMPAR_CRUFT
#define GIMP_DISABLE_COMPAR_CRUFT
#endif

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include <libgimp/stdplugins-intl.h>

#include "quadmath-modded.hpp"
#include <string>
#include <algorithm>
#include <cmath>

#include <omp.h>

#ifndef NULL_TERMINATE
// #define NULL_TERMINATE 0L
#define NULL_TERMINATE nullptr
// #define NULL_TERMINATE ((void*) 0)
#endif

namespace TritonhawkPlus {};

using namespace TritonhawkPlus;
using namespace std;
using namespace quadmath;

namespace TritonhawkPlus
{
    typedef float f32;
    typedef double f64;
    typedef __float128 f128;

    static inline f32 lerp32(f32 a, f32 b, f32 lerp_factor)
    {
        return (a * (1.0f - lerp_factor)) + (b * lerp_factor);
    }
    static inline f64 lerp64(f64 a, f64 b, f64 lerp_factor)
    {
        return (a * (1.0 - lerp_factor)) + (b * lerp_factor);
    }
    static inline f128 lerp128(f128 a, f128 b, f128 lerp_factor)
    {
        return (a * (1.0q - lerp_factor)) + (b * lerp_factor);
    }

    static inline f128 lerpq(f128 a, f128 b, f128 lerp_factor)
    {
        f128 lerp_fac = clamp01q(lerp_factor);
        return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
    }
    static inline f128 lerpexpq(f128 a, f128 b, f128 lerp_factor, f128 exponent)
    {
        f128 lerp_fac = powq(clamp01q(lerp_factor), exponent);
        return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
    }
    static inline f128 lerp2expq(f128 a, f128 b, f128 lerp_factor, f128 exponent)
    {
        f128 clamped = clamp01q(lerp_factor);
        f128 lerp_fac = (clamped >= 0.5Q) ? (powq(clamped * 2.Q, exponent) * 2.Q) : (1.Q - (powq((1.Q - clamped) * 2.Q, exponent) * 2.Q));
        return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
    }

    /*
    static inline string thp_get_special_param_text(gint value)
    {
        if (value == (gint)0)
            return "Disabled";
        else if (value == (gint)1)
            return "Mild";
        else if (value == (gint)2)
            return "Moderate";
        else if (value == (gint)3)
            return "Heavy";
        else if (value == (gint)4)
            return "Ultra";
        else
            return "-----";
    }
    */
}
