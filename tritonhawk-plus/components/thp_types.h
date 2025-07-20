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

namespace TritonhawkPlus {};
namespace quadmath {};

namespace TritonhawkPlus
{
    #undef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #if defined(__SIZEOF_LONG_DOUBLE__) && __SIZEOF_LONG_DOUBLE__ == 16
        #define THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #endif
    #if defined(__SIZEOF_FLOAT128__) || defined(__FLT128_IS_LONG_DOUBLE__)
        #define THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #endif

    #ifdef __cplusplus
        extern "C" {
    #endif
        using f32 = float;
        using f64 = double;
    #ifdef __cplusplus
        }
    #endif


    #ifdef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
        #ifdef __cplusplus
            extern "C" {
        #endif
            using f128 = long double;
        #ifdef __cplusplus
            }
        #endif

        constexpr long double operator ""_q(long double x)
        {
            return static_cast<long double>(x);
        };
    #else
        #ifdef __cplusplus
            extern "C" {
        #endif
            using f128 = __float128;
        #ifdef __cplusplus
            }
        #endif

        constexpr __float128 operator ""_q(__float128 x)
        {
            return static_cast<__float128>(x);
        };
    #endif
};

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

using namespace TritonhawkPlus;
using namespace std;
using namespace quadmath;

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
