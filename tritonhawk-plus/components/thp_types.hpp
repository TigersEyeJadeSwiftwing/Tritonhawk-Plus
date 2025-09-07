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

#define __STDC_WANT_IEC_60559_TYPES_EXT__ 1
#include <string>
#include <cmath>
// #include <stdfloat>
#include <cfloat>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <algorithm>

#include <omp.h>

#ifndef HOT_INLINE
    #define HOT_INLINE inline __attribute__((always_inline, hot))
#endif

namespace TritonhawkPlus {};
namespace quadmath {};
namespace intmath {};

namespace TritonhawkPlus
{
    #undef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #if defined(__SIZEOF_LONG_DOUBLE__) && __SIZEOF_LONG_DOUBLE__ == 16
        #define THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #endif
    #if defined(__SIZEOF_FLOAT128__) && defined(__FLT128_IS_LONG_DOUBLE__)
        #define THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #endif

    #if defined THP_FORCE_USE_FLOAT128
        #undef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #elif defined THP_FORCE_USE_LONG_DOUBLE_128
        #define THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #endif // defined

    #ifdef __cplusplus
        extern "C" {
    #endif
        using s8 = int8_t;
        using u8 = uint8_t;
        using s16 = int16_t;
        using u16 = uint16_t;
        using s32 = int32_t;
        using u32 = uint32_t;
        using s64 = int64_t;
        using u64 = uint64_t;

        using f16 = _Float16; /**< Uses predefined values and math library functions ending in "fs", which stands for "float, short" */
        using f32 = float; /**< Uses predefined values and math library functions ending in "f", which stands for "float" */
        using f64 = double; /**< Uses predefined values and math library functions ending in no extra letters, which is for only 64-bit doubles and does not have overloaded functions with custom functions in math library extensions. */
    #ifdef __cplusplus
        }
    #endif


    #ifdef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
        #ifdef __cplusplus
            extern "C" {
        #endif
            using f128 = long double; /**< Uses predefined values and math library functions ending in "q", which stands for "quad precision" */
        #ifdef __cplusplus
            }
        #endif

        constexpr f128 operator ""_q(long double x)
        {
            return static_cast<f128>(x);
        };
    #else
        #ifdef __cplusplus
            extern "C" {
        #endif
            using f128 = __float128; /**< Uses predefined values and math library functions ending in "q", which stands for "quad precision" */
        #ifdef __cplusplus
            }
        #endif

        // const f128 test1 = 10.q;
        // const f128 test2 = test1 + 5.q;

        constexpr __float128 operator ""_q(long double x)
        {
            return static_cast<__float128>(x);
        };
    #endif
};

#include "lib_int_math.hpp"
#include "quadmath-modded.hpp"

#include "config_h/clang64/config.hpp"

#ifndef GIMP_DISABLE_COMPAR_CRUFT
#define GIMP_DISABLE_COMPAR_CRUFT
#endif

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "config_h/clang64/stdplugins-intl.hpp"

/*
#include "lib_int_math.hpp"
#include <string>
#include <algorithm>
#include <cmath>
#include "quadmath-modded.hpp"

#include <omp.h>
*/

#ifndef NULL_TERMINATE
// #define NULL_TERMINATE 0L
#define NULL_TERMINATE nullptr
// #define NULL_TERMINATE ((void*) 0)
#endif

using namespace TritonhawkPlus;
using namespace std;
using namespace quadmath;
using namespace intmath;

/*
namespace TritonhawkPlus
{
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
}
*/
