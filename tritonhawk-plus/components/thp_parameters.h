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

#include "components/thp_types.h"
#include <string>

using namespace std;
using namespace TritonhawkPlus;

namespace TritonhawkPlus
{
    class ThpParams
    {
    public:
        string process_name = "";
        string info_string = "";
        int hardware_max_threads = 16;
        int preferences_max_threads = 16;
        int number_threads = 16;
        int draw_index = 0;
        int draw_count = 0;
        bool seamless_x = false;
        bool seamless_y = false;
        f128 sample_count_adjustment = 1.0_q;
        f64 sample_grid_height_percent = 100.0;
        f64 sample_grid_width_percent = 100.0;
        int sample_count_x = 1;
        int sample_count_y = 1;
        int sample_count_xy = 1;
        int total_samples = 1;
        f128 image_ratio_x = 1.0_q;
        f128 image_ratio_y = 1.0_q;
        f128 image_ratio_xy = 1.0_q;
        f128 sample_grid_scale_x = 1.0_q;
        f128 sample_grid_scale_y = 1.0_q;
        f128 sample_grid_offset_x = 0.0_q;
        f128 sample_grid_offset_y = 0.0_q;
        f128 sample_interpolation_x = 1.0_q;
        f128 sample_interpolation_y = 1.0_q;
        int chunk_size_default = 8192;
        int chunk_size_samples = 1;
        int chunk_size_pixels = 1;
        int number_chunks = 1;
        int input_size_x = 0;
        int input_size_y = 0;
        int input_size_xy = 0;
        int output_size_x = 0;
        int output_size_y = 0;
        int output_size_xy = 0;
        f64 progress_start = 0.0;
        f64 progress_end = 1.0;
        f64 progress_increment = 0.0;

        int max_image_dimension = 65536;

        void CalcAll();
        void CalcInfoString();
        void CalcThreads();
        void CalcSampleGrid();
        void CalcNumberOfChunks();
    };
};
