/*  Copyright (c) Tiger's Eye Jade Swiftwing, Master Tiger Dragon, Zarakite, all rights reserved.
    This file is written by Tiger's Eye Jade Swiftwing.  It is licensed under the GPLv3 license.  Note that my first name is "Tiger's Eye" (which is two words),
my middle name is "Jade", and "Swiftwing" is one word that is my last name.
    Tritonhawk-Plus is a creation of myself, Tiger's Eye Jade Swiftwing, also known as Tiger J. Swiftwing, Tiger Swiftwing, and a few other versions of my name.....
    I'm an American tiger dragon, SS# 287-74-8719.
    This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.  This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.  You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
    This file is part of the open-source Tritonhawk-Plus project(s), located at: https://github.com/TigersEyeJadeSwiftwing/Tritonhawk-Plus
    This software is one or more plug-ins for the open-source graphics and image software editing and creation programs known as GIMP.  GIMP can be found at:
https://www.gimp.org/
    If you want to borrow any of the source code from the custom math library .inl files that are part of this project, the ones with this copyright notice and such
are also licensed under the GPL version 3 license.  */

// #define WIN32_LEAN_AND_MEAN
// #define NOMINMAX
#include <windows.h>

#include "components/thp_types.hpp"
#include "components/thp_parameters.hpp"

#include <omp.h>

#include <string>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;
using namespace TritonhawkPlus;
using namespace quadmath;

namespace TritonhawkPlus
{
    TARGET_CLONES void ThpParams::SetPluginRealtime(bool realtime_active)
    {
        plugin_priority_realtime = realtime_active;
    }
    TARGET_CLONES void ThpParams::EngagePluginPriority()
    {
        if (plugin_priority_realtime == true)
        {
            SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
        }
        else
        {
            SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
        }
    }
    TARGET_CLONES void ThpParams::DisengagePluginPriority()
    {
        // SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
        SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
    }
    TARGET_CLONES void ThpParams::Reset()
    {
        plugin_priority_realtime = true;
        seamless_x = false;
        seamless_y = false;
        sample_count_adjustment = 1.0q;
        sample_grid_height_percent = 100.0;
        sample_grid_width_percent = 100.0;
        sample_interpolation_x = 1.0q;
        sample_interpolation_y = 1.0q;
        sample_grid_shape = SAMPLE_GRID_SHAPE_Auto;
        sample_grid_weighting = 0.q;
        // run_mode = RUN_MODE_RESIZE__ALL_LAYERS_SAME_RATIO_V2;
        // layers_to_process = LAYERS_TO_PROCESS__ACTIVE_SELECTION;
        // images_to_process = IMAGES_TO_PROCESS__CURRENT;
        chunk_size_kilo = 5uL;
        chunk_size_default = 1000uL * chunk_size_kilo;
        output_size_x = input_size_x;
        output_size_y = input_size_y;
        image_output_size_x = image_input_size_x;
        image_output_size_y = image_input_size_y;
        layer_output_size_x = layer_input_size_x;
        layer_output_size_y = layer_input_size_y;
        layer_is_full_frame = true;

        CalcAll();
    }
    TARGET_CLONES void ThpParams::SetOutputSizeDefaults()
    {
        if (sample_grid_is_for_all_layers == true)
        {
            input_size_x = image_input_size_x;
            input_size_y = image_input_size_y;
            input_size_xy = image_input_size_xy;
            output_size_x = image_output_size_x;
            output_size_y = image_output_size_y;
            output_size_xy = image_output_size_xy;
        }
        else
        {
            input_size_x = layer_input_size_x;
            input_size_y = layer_input_size_y;
            input_size_xy = layer_input_size_xy;
            output_size_x = layer_output_size_x;
            output_size_y = layer_output_size_y;
            output_size_xy = layer_output_size_xy;
        }
    }
    TARGET_CLONES void ThpParams::CopySizesToLayerSizes()
    {
        layer_input_size_x = input_size_x;
        layer_input_size_y = input_size_y;
        layer_input_size_xy = input_size_xy;
        layer_output_size_x = output_size_x;
        layer_output_size_y = output_size_y;
        layer_output_size_xy = output_size_xy;

        layer_ratio_x = f128(layer_input_size_x) / f128(layer_output_size_x);
        layer_ratio_y = f128(layer_input_size_y) / f128(layer_output_size_y);
        layer_ratio_xy = f128(layer_input_size_xy) / f128(layer_output_size_xy);
    }
    TARGET_CLONES void ThpParams::CalcAll()
    {
        CalcThreads();

        CalcSampleGrid();
        CalcNumberOfChunks();

        /*
        CalcNumberOfChunks();
        CalcSampleGrid();
        CalcNumberOfChunks();
        CalcSampleGrid();
        CalcNumberOfChunks();
        */

        CalcInfoString();
        // CalcInfoString();
    }
    TARGET_CLONES void ThpParams::CalcInfoString()
    {
        info_string = (string) g_strdup_printf (
            _(
                "Tritonhawk Plus GIMP Plug-in: %s" "\n"
                "   High Quality, Multi-threaded, using OpenMP technology." "\n"
                "   Uses 128-bit floating-point math for processing." "\n"
                "\n"
                "Enabled threads for multi-threading: %i, %s" "\n"
                "Number of drawables to process in image: %i" "\n"
                "   Image input size: %I64u x %I64u px, %I64u total pixels" "\n"
                "   Image output size: %I64u x %I64u px, %I64u total pixels" "\n"
                "   Layer input size: %I64u x %I64u px, %I64u total pixels" "\n"
                "   Layer output size: %I64u x %I64u px, %I64u total pixels" "\n"
                "Seamless tiling X: %s, Seamless tiling Y: %s" "\n"
                "Sample size adjustment level: %%%9.2lf" "\n"
                "   Sample grid size: %I64u x %I64u, total samples: %I64u" "\n"
                "   Sample grid scale: %%%9.2lf x %%%9.2lf" "\n"
                "Default Chunk size: %I64u pixels (%I64u samples)" "\n"
                "   Actual Chunk size: %I64u pixels (%I64u samples)" "\n"
                "   Number of chunks to process: %I64u (%I64u samples)"
            ),
            process_name.c_str(),
            number_threads, plugin_priority_realtime ? "Realtime" : "Background",
            draw_count,
            image_input_size_x, image_input_size_y, image_input_size_xy,
            image_output_size_x, image_output_size_y, image_output_size_xy,
            layer_input_size_x, layer_input_size_y, layer_input_size_xy,
            layer_output_size_x, layer_output_size_y, layer_output_size_xy,
            seamless_x ? "Enabled" : "Inactive", seamless_y ? "Enabled" : "Inactive",
            f64(sample_count_adjustment * 100.q),
            sample_count_x, sample_count_y, sample_count_xy,
            f64(sample_grid_scale_x * 100.q), f64(sample_grid_scale_y * 100.q),
            chunk_size_default, u64(chunk_size_default * sample_count_xy),
            chunk_size_pixels, chunk_size_samples,
            number_chunks, total_samples
        );
    }
    TARGET_CLONES void ThpParams::CalcThreads()
    {
        // s16 max_number_threads = min(hardware_max_threads, preferences_max_threads);
        // number_threads = min(number_threads, max_number_threads);

        number_threads = min(number_threads, hardware_max_threads * s16(16));

        omp_set_num_threads( (int)number_threads );
    }
    TARGET_CLONES void ThpParams::CalcSampleGrid()
    {
        input_size_x = min(max(input_size_x, 1uL), max_image_dimension);
        input_size_y = min(max(input_size_y, 1uL), max_image_dimension);
        input_size_xy = input_size_x * input_size_y;
        output_size_x = min(max(output_size_x, 1uL), max_image_dimension);
        output_size_y = min(max(output_size_y, 1uL), max_image_dimension);
        output_size_xy = output_size_x * output_size_y;

        f128 ratio_x = f128(input_size_x) / f128(output_size_x);
        f128 ratio_y = f128(input_size_y) / f128(output_size_y);
        // f128 ratio_xy = f128(input_size_xy) / f128(output_size_xy);

        // Temporary
        /*
        {
            image_input_size_x = input_size_x;
            image_input_size_y = input_size_y;
            layer_input_size_x = input_size_x;
            layer_input_size_y = input_size_y;

            image_output_size_x = output_size_x;
            image_output_size_y = output_size_y;
            layer_output_size_x = output_size_x;
            layer_output_size_y = output_size_y;
        }
        */

        image_input_size_x = clamp(image_input_size_x, 1uLL, max_image_dimension);
        image_input_size_y = clamp(image_input_size_y, 1uLL, max_image_dimension);
        image_input_size_xy = image_input_size_x * image_input_size_y;
        image_output_size_x = clamp(image_output_size_x, 1uLL, max_image_dimension);
        image_output_size_y = clamp(image_output_size_y, 1uLL, max_image_dimension);
        image_output_size_xy = image_output_size_x * image_output_size_y;

        layer_input_size_x = clamp(layer_input_size_x, 1uLL, max_image_dimension);
        layer_input_size_y = clamp(layer_input_size_y, 1uLL, max_image_dimension);
        layer_input_size_xy = layer_input_size_x * layer_input_size_y;
        layer_output_size_x = clamp(layer_output_size_x, 1uLL, max_image_dimension);
        layer_output_size_y = clamp(layer_output_size_y, 1uLL, max_image_dimension);
        layer_output_size_xy = layer_output_size_x * layer_output_size_y;

        image_ratio_x = f128(image_input_size_x) / f128(image_output_size_x);
        image_ratio_y = f128(image_input_size_y) / f128(image_output_size_y);
        image_ratio_xy = f128(image_input_size_xy) / f128(image_output_size_xy);
        layer_ratio_x = f128(layer_input_size_x) / f128(layer_output_size_x);
        layer_ratio_y = f128(layer_input_size_y) / f128(layer_output_size_y);
        layer_ratio_xy = f128(layer_input_size_xy) / f128(layer_output_size_xy);

        // f128 ratio_x = 1.0q, ratio_y = 1.0q;

        /*
        if (sample_grid_is_for_all_layers == true)
        {
            input_size_x = image_input_size_x;
            input_size_y = image_input_size_y;
            input_size_xy = image_input_size_xy;
            output_size_x = image_output_size_x;
            output_size_y = image_output_size_y;
            output_size_xy = image_output_size_xy;

            ratio_x = image_ratio_x;
            ratio_y = image_ratio_y;
        }
        else
        {
            input_size_x = layer_input_size_x;
            input_size_y = layer_input_size_y;
            input_size_xy = layer_input_size_xy;
            output_size_x = layer_output_size_x;
            output_size_y = layer_output_size_y;
            output_size_xy = layer_output_size_xy;

            ratio_x = layer_ratio_x;
            ratio_y = layer_ratio_y;
        }
        */

        sample_count_x = 1;
        sample_count_y = 1;
        sample_count_xy = 1;
        f128 grid_scale_x = f128(sample_grid_width_percent) * 0.01q;
        f128 grid_scale_y = f128(sample_grid_height_percent) * 0.01q;
        sample_grid_scale_x = 1.0q;
        sample_grid_scale_y = 1.0q;
        sample_grid_offset_x = 0.5q;
        sample_grid_offset_y = 0.5q;

        if (sample_grid_shape == SAMPLE_GRID_SHAPE_Square)
        {
            sample_grid_shape_x = SAMPLE_GRID_SHAPE_Square;
            sample_grid_shape_y = SAMPLE_GRID_SHAPE_Square;
        }
        else if (sample_grid_shape == SAMPLE_GRID_SHAPE_Circle)
        {
            sample_grid_shape_x = SAMPLE_GRID_SHAPE_Circle;
            sample_grid_shape_y = SAMPLE_GRID_SHAPE_Circle;
        }
        else if (sample_grid_shape == SAMPLE_GRID_SHAPE_Auto)
        {
            sample_grid_shape_x = SAMPLE_GRID_SHAPE_Circle;
            sample_grid_shape_y = SAMPLE_GRID_SHAPE_Circle;

            if (input_size_x >= output_size_x)
                sample_grid_shape_x = SAMPLE_GRID_SHAPE_Square;
            if (input_size_y >= output_size_y)
                sample_grid_shape_y = SAMPLE_GRID_SHAPE_Square;

            /*
            if ((input_size_x == output_size_x) && (input_size_y == output_size_y))
            {
                sample_grid_shape_x = SAMPLE_GRID_SHAPE_Square;
                sample_grid_shape_y = SAMPLE_GRID_SHAPE_Square;
            }
            */
        }
        else
        {
            sample_grid_shape_x = SAMPLE_GRID_SHAPE_Square;
            sample_grid_shape_y = SAMPLE_GRID_SHAPE_Square;
        }

        f128 shape_scale_factor_x = (sample_grid_shape_x == SAMPLE_GRID_SHAPE_Circle) ? M_SQRT2q : 1.0q;
        f128 shape_scale_factor_y = (sample_grid_shape_y == SAMPLE_GRID_SHAPE_Circle) ? M_SQRT2q : 1.0q;

        // Shrinking
        if (input_size_x > output_size_x)
        {

            f128 scale_factor = f128 (grid_scale_x * ratio_x * shape_scale_factor_x) / 2.0q;
            u64 additional_samples = u64 (scale_factor) * 2u;
            sample_count_x = 3u + additional_samples;
            sample_grid_scale_x = grid_scale_x * ratio_x * shape_scale_factor_x;

            sample_grid_offset_x = 0.5q * ratio_x;
        }
        // Growing
        else if (input_size_x < output_size_x)
        {
            if ((grid_scale_x > 1.00005q) || (sample_grid_shape_x != SAMPLE_GRID_SHAPE_Square))
            {
                f128 scale_factor = f128 (grid_scale_x * shape_scale_factor_x) * 0.5q;
                u64 additional_samples = u64 (scale_factor) * 2u;
                sample_count_x = 3u + additional_samples;
                sample_grid_scale_x = (grid_scale_x * shape_scale_factor_x) - 1.0q;
            }
            else
            {
                sample_count_x = 1u;
                sample_grid_scale_x = 0.0q;
            }

            sample_grid_offset_x = 0.5q * ratio_x;
        }
        // No change
        else
        {
            if ((grid_scale_x > 1.00005q) || (sample_grid_shape_x != SAMPLE_GRID_SHAPE_Square))
            {
                f128 scale_factor = f128 (grid_scale_x * shape_scale_factor_x) * 0.5q;
                u64 additional_samples = u64 (scale_factor) * 2u;
                sample_count_x = 3u + additional_samples;
                sample_grid_scale_x = (grid_scale_x * shape_scale_factor_x) - 1.0q;
            }
            else
            {
                sample_count_x = 1u;
                sample_grid_scale_x = 0.0q;
            }

            sample_grid_offset_x = 0.5q;
        }

        // Shrinking
        if (input_size_y > output_size_y)
        {
            f128 scale_factor = f128 (grid_scale_y * ratio_y * shape_scale_factor_y) / 2.0q;
            u64 additional_samples = u64 (scale_factor) * 2u;
            sample_count_y = 3u + additional_samples;
            sample_grid_scale_y = grid_scale_y * ratio_y * shape_scale_factor_y;

            sample_grid_offset_y = 0.5q * ratio_y;
        }
        // Growing
        else if (input_size_y < output_size_y)
        {
            if ((grid_scale_y > 1.00005q) || (sample_grid_shape_y != SAMPLE_GRID_SHAPE_Square))
            {
                f128 scale_factor = f128 (grid_scale_y * shape_scale_factor_y) * 0.5q;
                u64 additional_samples = u64 (scale_factor) * 2u;
                sample_count_y = 3u + additional_samples;
                sample_grid_scale_y = (grid_scale_y * shape_scale_factor_y) - 1.0q;
            }
            else
            {
                sample_count_y = 1u;
                sample_grid_scale_y = 0.0q;
            }

            sample_grid_offset_y = 0.5q * ratio_y;
        }
        // No change
        else
        {
            if ((grid_scale_y > 1.00005q) || (sample_grid_shape_y != SAMPLE_GRID_SHAPE_Square))
            {
                f128 scale_factor = f128 (grid_scale_y * shape_scale_factor_y) * 0.5q;
                u64 additional_samples = u64 (scale_factor) * 2u;
                sample_count_y = 3u + additional_samples;
                sample_grid_scale_y = (grid_scale_y * shape_scale_factor_y) - 1.0q;
            }
            else
            {
                sample_count_y = 1u;
                sample_grid_scale_y = 0.0q;
            }

            sample_grid_offset_y = 0.5q;
        }

        if ((sample_count_adjustment > 1.0005q) || (sample_count_adjustment < 0.9995q))
        {
            f128 s_c_xf = fmaxq(f128(sample_count_x) * sample_count_adjustment, 1.00005q);
            f128 s_c_yf = fmaxq(f128(sample_count_y) * sample_count_adjustment, 1.00005q);
            sample_count_x = (u64) max((u64)to_intq(s_c_xf), 1uL);
            sample_count_y = (u64) max((u64)to_intq(s_c_yf), 1uL);
        }

        sample_count_xy = sample_count_x * sample_count_y;
    }

    TARGET_CLONES void ThpParams::GetSampleGridVectors(vector<SampleGridElement> *grid)
    {
        grid->clear();
        grid->resize(sample_count_xy, SampleGridElement());

        if ((sample_grid_size_x != 0uL) && (sample_grid_size_y != 0uL))
        {
            if ((sample_count_x == sample_grid_size_x) &&
                (sample_count_y == sample_grid_size_y) &&
                (sample_grid_cached_shape_x == sample_grid_shape_x) &&
                (sample_grid_cached_shape_y == sample_grid_shape_y))
            {
                grid->assign(sample_grid.begin(), sample_grid.end());
                return;
            }
        }

        #pragma omp parallel for shared(grid) default(firstprivate)
        for (u64 index = 0uLL; index < sample_count_xy; index++)
        {
            u64 px = index % sample_count_x;
            u64 py = index / sample_count_x;

            f128 spos_x = 0.0q;
            f128 spos_y = 0.0q;

            if (sample_count_x > 1uLL)
                spos_x = 2.0q * ((f128(px) / f128(sample_count_x - 1uLL)) - 0.5q);
            if (sample_count_y > 1uLL)
                spos_y = 2.0q * ((f128(py) / f128(sample_count_y - 1uLL)) - 0.5q);

            if ((sample_grid_shape_x == SAMPLE_GRID_SHAPE_Square) && (sample_grid_shape_y == SAMPLE_GRID_SHAPE_Square))
            {
                #pragma omp atomic write
                grid->at(index).x = spos_x * 0.5q;
                #pragma omp atomic write
                grid->at(index).y = spos_y * 0.5q;
            }
            else
            {
                f128 hyp = sqrtq((spos_x * spos_x) + (spos_y * spos_y));
                f128 position_ellipse_x = spos_x;
                f128 position_ellipse_y = spos_y;

                if (hyp > 0.000005q)
                {
                    f128 abs_x = fabsq(spos_x);
                    f128 abs_y = fabsq(spos_y);

                    if (abs_x > 0.000005q)
                    {
                        f128 hyp_diff_factor = fmodq((fmaxq(hyp - abs_x, 0.0q) / abs_x) * SMP_GRID_ELLIPSE_COS_FACTOR, M_PIq);
                        position_ellipse_x = ((cosq(hyp_diff_factor) * M_SQRT2_m1q) + 1.0q) * spos_x;
                    }

                    if (abs_y > 0.000005q)
                    {
                        f128 hyp_diff_factor = fmodq((fmaxq(hyp - abs_y, 0.0q) / abs_y) * SMP_GRID_ELLIPSE_COS_FACTOR, M_PIq);
                        position_ellipse_y = ((cosq(hyp_diff_factor) * M_SQRT2_m1q) + 1.0q) * spos_y;
                    }
                }

                #pragma omp atomic write
                grid->at(index).x = (sample_grid_shape_x == SAMPLE_GRID_SHAPE_Square) ? spos_x * 0.5q : position_ellipse_x * 0.5q;
                #pragma omp atomic write
                grid->at(index).y = (sample_grid_shape_y == SAMPLE_GRID_SHAPE_Square) ? spos_y * 0.5q : position_ellipse_y * 0.5q;
            }

            if ((sample_grid_weighting < 0.000005q) || (sample_count_xy < 2uLL))
            {
                #pragma omp atomic write
                grid->at(index).weight = 1.0q;
            }
            else
            {
                f128 gx = spos_x, gy = spos_y;

                /*
                #pragma omp atomic read
                gx = grid->at(index).x;
                #pragma omp atomic read
                gy = grid->at(index).y;
                */

                f128 hyp = sqrtq((gx * gx) + (gy * gy));
                f128 weight_factor = clamp01q(hyp / M_SQRT2q);
                f128 sample_weight_base = cosq(weight_factor * M_PI_2q);
                f128 sample_weight = lerpq(1.0q, sample_weight_base, sample_grid_weighting);

                #pragma omp atomic write
                grid->at(index).weight = sample_weight;
            }

            #pragma omp atomic update
            grid->at(index).x *= sample_grid_scale_x;
            #pragma omp atomic update
            grid->at(index).y *= sample_grid_scale_y;
            #pragma omp atomic update
            grid->at(index).x += sample_grid_offset_x;
            #pragma omp atomic update
            grid->at(index).y += sample_grid_offset_y;
        }

        sample_grid_size_x = sample_count_x;
        sample_grid_size_y = sample_count_y;
        sample_grid_cached_shape_x = sample_grid_shape_x;
        sample_grid_cached_shape_y = sample_grid_shape_y;
        sample_grid.assign(grid->begin(), grid->end());
    }

    TARGET_CLONES void ThpParams::CalcNumberOfChunks()
    {
        sample_count_xy = max(sample_count_xy, 1uLL);
        chunk_size_default = max(1000uL * chunk_size_kilo, 1000uL);
        chunk_size_samples = chunk_size_default;
        input_size_x = clamp(input_size_x, 1uL, max_image_dimension);
        input_size_y = clamp(input_size_y, 1uL, max_image_dimension);
        input_size_xy = input_size_x * input_size_y;
        output_size_x = clamp(output_size_x, 1uL, max_image_dimension);
        output_size_y = clamp(output_size_y, 1uL, max_image_dimension);
        output_size_xy = output_size_x * output_size_y;
        image_ratio_x = f128(input_size_x) / f128(output_size_x);
        image_ratio_y = f128(input_size_y) / f128(output_size_y);
        image_ratio_xy = f128(input_size_xy) / f128(output_size_xy);

        total_samples = output_size_xy * sample_count_xy;
        chunk_size_pixels = max(chunk_size_samples / sample_count_xy, 1uL) + 1uL;
        number_chunks = (output_size_xy / chunk_size_pixels) + 1uL;
        if (number_chunks < number_threads)
        {
            f128 chunk_size_ratio = f128(number_threads) / f128(number_chunks);
            f128 chunk_size_pixels_f = f128(chunk_size_pixels) / chunk_size_ratio;
            chunk_size_pixels = int(chunk_size_pixels_f) + 1uL;
            number_chunks = (output_size_xy / chunk_size_pixels) + 1uL;
        }
        if ((number_chunks * chunk_size_pixels) < output_size_xy)
        {
            int pixels_diff = output_size_xy - (number_chunks * chunk_size_pixels);
            int chunks_diff = pixels_diff / chunk_size_pixels;
            number_chunks += chunks_diff + 1uL;
        }

        progress_increment = (progress_end - progress_start) / f64(number_chunks);
        // progress_increments = 1.0 / progress_increment;
        // progress_increments = f64(number_chunks) / fmax(f64(progress_end) - f64(progress_start), 1.0);
    }

    TARGET_CLONES void ThpParams::SetResizeParameters_NonInteractive(GimpProcedureConfig* Config)
    {
        if (!Config) return;

        gint new_x_config = 0;
        gint new_y_config = 0;
        gdouble new_x_percent_config = 0.0;
        gdouble new_y_percent_config = 0.0;
        gint threads_ = 0;
        gboolean background_ = FALSE;
        gchar* sample_grid_shape_config;
        gdouble sample_grid_x_ = 0.0;
        gdouble sample_grid_y_ = 0.0;
        gboolean seamless_x_ = FALSE;
        gboolean seamless_y_ = FALSE;
        gdouble sample_grid_weighting_ = 0.0;
        gdouble sample_grid_count_ = 0.0;
        gdouble sample_interpolation_x_ = 0.0;
        gdouble sample_interpolation_y_ = 0.0;
        gint chunk_size_config = 0;
        gchar* resize_mode_;

        g_object_get(Config,
                     "new-x", &new_x_config,
                     "new-y", &new_y_config,
                     "new-x-percent", &new_x_percent_config,
                     "new-y-percent", &new_y_percent_config,
                     "threads", &threads_,
                     "background", &background_,
                     "sample-grid-shape", &sample_grid_shape_config,
                     "sample-grid-x", &sample_grid_x_,
                     "sample-grid-y", &sample_grid_y_,
                     "seamless-x", &seamless_x_,
                     "seamless-y", &seamless_y_,
                     "sample-grid-weighting", &sample_grid_weighting_,
                     "sample-grid-count", &sample_grid_count_,
                     "sample-interpolation-x", &sample_interpolation_x_,
                     "sample-interpolation-y", &sample_interpolation_y_,
                     "chunk-size", &chunk_size_config,
                     "resize-mode", &resize_mode_,
                     NULL_TERMINATE);

        output_size_x = (u64)new_x_config;
        output_size_y = (u64)new_y_config;
        image_output_size_x = (u64)new_x_config;
        image_output_size_y = (u64)new_y_config;
        layer_output_size_x = (u64)new_x_config;
        layer_output_size_y = (u64)new_y_config;

        image_ratio_x = new_x_percent_config;
        image_ratio_y = new_y_percent_config;
        layer_ratio_x = new_x_percent_config;
        layer_ratio_y = new_y_percent_config;

        if (new_x_config == 0)
        {
            new_x_config = gint(f128((f128)new_x_percent_config * (f128)input_size_x * 0.01q));
            new_x_config = (gint)max(new_x_config, (gint)1);
            output_size_x = (u64)new_x_config;
            image_output_size_x = (u64)new_x_config;
            layer_output_size_x = (u64)new_x_config;
        }
        else
        {
            new_x_percent_config = f128(new_x_config / (f128)layer_input_size_x) * 100.0q;
            new_x_percent_config = fmaxq(new_x_percent_config, 0.0000001q);
            image_ratio_x = new_x_percent_config;
            layer_ratio_x = new_x_percent_config;
        }
        if (new_y_config == 0)
        {
            new_y_config = gint(f128((f128)new_y_percent_config * (f128)input_size_y) * 0.01q);
            new_y_config = (gint)max(new_y_config, (gint)1);
            output_size_y = (u64)new_y_config;
            image_output_size_y = (u64)new_y_config;
            layer_output_size_y = (u64)new_y_config;
        }
        else
        {
            new_y_percent_config = f128(new_y_config / (f128)layer_input_size_y) * 100.0q;
            new_y_percent_config = fmaxq(new_y_percent_config, 0.0000001q);
            image_ratio_y = new_y_percent_config;
            layer_ratio_y = new_y_percent_config;
        }

        output_size_xy = output_size_x * output_size_y;
        image_output_size_xy = image_output_size_x * image_output_size_y;
        layer_output_size_xy = layer_output_size_x * layer_output_size_y;

        number_threads = min(threads_, hardware_max_threads * s16(16));
        omp_set_num_threads( (int)number_threads );

        if (background_ == TRUE)
            plugin_priority_realtime = false;
        else
            plugin_priority_realtime = true;

        if (g_strcmp0 (sample_grid_shape_config, "auto") == 0)
            sample_grid_shape = SAMPLE_GRID_SHAPE_Auto;
        else if (g_strcmp0 (sample_grid_shape_config, "Auto") == 0)
            sample_grid_shape = SAMPLE_GRID_SHAPE_Auto;
        else if (g_strcmp0 (sample_grid_shape_config, "AUTO") == 0)
            sample_grid_shape = SAMPLE_GRID_SHAPE_Auto;
        else if (g_strcmp0 (sample_grid_shape_config, "square") == 0)
            sample_grid_shape = SAMPLE_GRID_SHAPE_Square;
        else if (g_strcmp0 (sample_grid_shape_config, "Square") == 0)
            sample_grid_shape = SAMPLE_GRID_SHAPE_Square;
        else if (g_strcmp0 (sample_grid_shape_config, "SQUARE") == 0)
            sample_grid_shape = SAMPLE_GRID_SHAPE_Square;
        else if (g_strcmp0 (sample_grid_shape_config, "circle") == 0)
            sample_grid_shape = SAMPLE_GRID_SHAPE_Circle;
        else if (g_strcmp0 (sample_grid_shape_config, "Circle") == 0)
            sample_grid_shape = SAMPLE_GRID_SHAPE_Circle;
        else if (g_strcmp0 (sample_grid_shape_config, "CIRCLE") == 0)
            sample_grid_shape = SAMPLE_GRID_SHAPE_Circle;
        else
            sample_grid_shape = SAMPLE_GRID_SHAPE_Auto;

        sample_grid_scale_x = sample_grid_x_ * 0.01q;
        sample_grid_scale_y = sample_grid_y_ * 0.01q;

        if (seamless_x_ == TRUE)
            seamless_x = true;
        else
            seamless_x = false;
        if (seamless_y_ == TRUE)
            seamless_y = true;
        else
            seamless_y = false;

        sample_grid_weighting = sample_grid_weighting_;
        sample_count_adjustment = sample_grid_count_ * 0.01q;
        sample_interpolation_x = sample_interpolation_x_;
        sample_interpolation_y = sample_interpolation_y_;

        chunk_size_kilo = (u64)chunk_size_config;
        chunk_size_pixels = (u64)chunk_size_config * 1000uL;
        chunk_size_samples = (u64)chunk_size_config * 1000uL;

        if (whole_image_mode == false)
        {
            run_mode = RUN_MODE_RESIZE__ALL_LAYERS_SAME_DIMENSIONS_V2;
        }
        else if ((g_strcmp0 (sample_grid_shape_config, "same-ratio") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "Same-Ratio") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "SAME-RATIO") == 0) )
        {
            run_mode = RUN_MODE_RESIZE__ALL_LAYERS_SAME_RATIO_V2;
        }
        else if ((g_strcmp0 (sample_grid_shape_config, "same-dimensions") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "Same-Dimensions") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "SAME-DIMENSIONS") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "same-size") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "Same-Size") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "SAME-SIZE") == 0) )
        {
            run_mode = RUN_MODE_RESIZE__ALL_LAYERS_SAME_DIMENSIONS_V2;
        }
        else if ((g_strcmp0 (sample_grid_shape_config, "same-width") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "Same-Width") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "SAME-WIDTH") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "same-x") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "Same-X") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "SAME-X") == 0) )
        {
            run_mode = RUN_MODE_RESIZE__KEEP_ASPECT_SAME_HORIZONTAL_V2;
        }
        else if ((g_strcmp0 (sample_grid_shape_config, "same-height") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "Same-Height") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "SAME-HEIGHT") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "same-y") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "Same-Y") == 0) ||
                (g_strcmp0 (sample_grid_shape_config, "SAME-Y") == 0) )
        {
            run_mode = RUN_MODE_RESIZE__KEEP_ASPECT_SAME_VERTICAL_V2;
        }
    }
};
