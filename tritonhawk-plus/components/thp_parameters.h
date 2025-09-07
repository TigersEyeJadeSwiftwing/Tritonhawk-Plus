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

#include "config_h/clang64/config.h"

#ifndef GIMP_DISABLE_COMPAR_CRUFT
#define GIMP_DISABLE_COMPAR_CRUFT
#endif

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "components/thp_types.h"
#include <string>
#include <vector>

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

using namespace std;
using namespace TritonhawkPlus;

namespace TritonhawkPlus
{
    enum SAMPLE_GRID_SHAPE : u8
    {
        SAMPLE_GRID_SHAPE_Square,
        SAMPLE_GRID_SHAPE_Circle,
        SAMPLE_GRID_SHAPE_Auto,
        SAMPLE_GRID_SHAPE_MAX_COUNT
    };

    struct SampleGridElement
    {
        f128 x = 0._q;
        f128 y = 0._q;
        f128 weight = 1._q;

        SampleGridElement()
        {
            x = 0._q;
            y = 0._q;
            weight = 1._q;
        }
        SampleGridElement(f128 px, f128 py)
        {
            x = px;
            y = py;
            weight = 1._q;
        }
        SampleGridElement(f128 px, f128 py, f128 pw)
        {
            x = px;
            y = py;
            weight = pw;
        }
    };

    class ThpParams
    {
    public:
        string process_name = "";
        string info_string = "";
        s16 hardware_max_threads = 16;
        s16 preferences_max_threads = 16;
        s16 number_threads = 16;
        s32 draw_index = 0;
        s32 draw_count = 0;
        bool seamless_x = false;
        bool seamless_y = false;
        f128 sample_count_adjustment = 1.0_q;
        f64 sample_grid_height_percent = 100.0;
        f64 sample_grid_width_percent = 100.0;
        u64 sample_count_x = 1u;
        u64 sample_count_y = 1u;
        u64 sample_count_xy = 1u;
        u64 total_samples = 1u;
        f128 image_ratio_x = 1.0_q;
        f128 image_ratio_y = 1.0_q;
        f128 image_ratio_xy = 1.0_q;
        f128 sample_grid_scale_x = 1.0_q;
        f128 sample_grid_scale_y = 1.0_q;
        f128 sample_grid_offset_x = 0.0_q;
        f128 sample_grid_offset_y = 0.0_q;
        f128 sample_interpolation_x = 1.0_q;
        f128 sample_interpolation_y = 1.0_q;
        SAMPLE_GRID_SHAPE sample_grid_shape = SAMPLE_GRID_SHAPE_Auto;
        SAMPLE_GRID_SHAPE sample_grid_shape_x = SAMPLE_GRID_SHAPE_Square;
        SAMPLE_GRID_SHAPE sample_grid_shape_y = SAMPLE_GRID_SHAPE_Square;
        f128 sample_grid_weighting = 0._q;
        u64 chunk_size_kilo = 256uL;
        u64 chunk_size_default = 1024uL * 128uL;
        u64 chunk_size_samples = 1u;
        u64 chunk_size_pixels = 1u;
        u64 number_chunks = 1u;
        u64 input_size_x = 256uL;
        u64 input_size_y = 256uL;
        u64 input_size_xy = 256uL * 256uL;
        u64 output_size_x = 256uL;
        u64 output_size_y = 256uL;
        u64 output_size_xy = 256uL * 256uL;
        f64 progress_start = 0.0;
        f64 progress_end = 1.0;
        f64 progress_increment = 0.0;

        bool gui_enabled = false;

        u64 max_image_dimension = 65536uL;
        f64 max_sample_grid_dimension_percent = 5000.0;
        f64 max_sample_count_modifier = 5000.0;
        f64 max_sample_interpolation = 100.0;
        u64 min_chunk_size = 32uL;
        u64 max_chunk_size = 65536uL;
        u64 increment_chunk_size = 32uL;

        void Reset();
        void CalcAll();
        void CalcInfoString();
        void CalcThreads();
        void CalcSampleGrid();
        void CalcNumberOfChunks();
        void GetSampleGridVectors(vector<SampleGridElement> *grid);
    };
};
