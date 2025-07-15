#pragma once

/*
Copyright (c) Tiger's Eye Jade Swiftwing, all rights reserved.

This file is written by Tiger's Eye Jade Swiftwing.  It is licensed under the GPLv3 license.
Note that my first name is "Tiger's Eye" (which is two words), my middle name is "Jade", and "Swiftwing" is one word that is my last name.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/


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
