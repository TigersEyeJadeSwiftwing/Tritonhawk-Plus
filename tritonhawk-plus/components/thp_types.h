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
