#pragma once

#include "config_h/clang64/config.h"

#ifndef GIMP_DISABLE_COMPAR_CRUFT
#define GIMP_DISABLE_COMPAR_CRUFT
#endif

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include <libgimp/stdplugins-intl.h>

#include "components/thp_types.h"
#include "components/thp_parameters.h"

namespace TritonhawkPlus
{
    void Thp_Resize_drawable_RGBA(ThpParams* params, GimpDrawable* draw_0, GimpDrawable* draw_1);
    void Thp_Resize_drawable_RGB(ThpParams* params, GimpDrawable* draw_0, GimpDrawable* draw_1);
};
