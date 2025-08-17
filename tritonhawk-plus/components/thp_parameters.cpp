#include "components/thp_types.h"
#include "components/thp_parameters.h"

#include <omp.h>

#include <string>
#include <algorithm>
#include <cmath>
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
using namespace quadmath;

namespace TritonhawkPlus
{
    void ThpParams::Reset()
    {
        seamless_x = false;
        seamless_y = false;
        sample_count_adjustment = 1.0_q;
        sample_grid_height_percent = 100.0;
        sample_grid_width_percent = 100.0;
        sample_interpolation_x = 3.0_q;
        sample_interpolation_y = 3.0_q;
        sample_grid_shape = SAMPLE_GRID_SHAPE_Auto;
        sample_grid_weighting = 0._q;
        chunk_size_kilo = 128uL;
        chunk_size_default = 1024uL * chunk_size_kilo;
        output_size_x = input_size_x;
        output_size_y = input_size_y;

        CalcAll();
    }
    void ThpParams::CalcAll()
    {
        CalcThreads();
        CalcNumberOfChunks();
        CalcSampleGrid();
        CalcNumberOfChunks();
        CalcInfoString();
    }
    void ThpParams::CalcInfoString()
    {
        info_string = (string) g_strdup_printf (
            _(
                "Tritonhawk Plus GIMP Plug-in: %s" "\n"
                "   High Quality, Multi-threaded, using OpenMP technology." "\n"
                "   Uses 128-bit floating-point math for processing." "\n"
                "\n"
                "Enabled threads for multi-threaded: %i" "\n"
                "Number of drawables to process in image: %i" "\n"
                "   Old image size: %I64u x %I64u pixels, %I64u total pixels" "\n"
                "   New image size: %I64u x %I64u pixels, %I64u total pixels" "\n"
                "Seamless tiling X: %s, Seamless tiling Y: %s" "\n"
                "Sample size adjustment level: %%%9.2lf" "\n"
                "   Sample grid size: %I64u x %I64u, total samples: %I64u" "\n"
                "   Sample grid scale: %%%9.5lf x %%%9.5lf" "\n"
                "Default Chunk size: %I64u pixels (%I64u samples)" "\n"
                "   Actual Chunk size: %I64u pixels (%I64u samples)" "\n"
                "   Number of chunks to process: %I64u (%I64u samples)" "\n"
            ),
            process_name.c_str(),
            number_threads,
            draw_count,
            input_size_x, input_size_y, input_size_xy,
            output_size_x, output_size_y, output_size_xy,
            seamless_x ? "Enabled" : "Inactive", seamless_y ? "Enabled" : "Inactive",
            f64(sample_count_adjustment * 100._q),
            sample_count_x, sample_count_y, sample_count_xy,
            f64(sample_grid_scale_x * 100._q), f64(sample_grid_scale_y * 100._q),
            chunk_size_default, u64(chunk_size_default * sample_count_xy),
            chunk_size_pixels, chunk_size_samples,
            number_chunks, total_samples
        );
    }
    void ThpParams::CalcThreads()
    {
        number_threads = min(hardware_max_threads, preferences_max_threads);
    }
    void ThpParams::CalcSampleGrid()
    {
        input_size_x = min(max(input_size_x, 1), max_image_dimension);
        input_size_y = min(max(input_size_y, 1), max_image_dimension);
        input_size_xy = input_size_x * input_size_y;
        output_size_x = min(max(output_size_x, 1), max_image_dimension);
        output_size_y = min(max(output_size_y, 1), max_image_dimension);
        output_size_xy = output_size_x * output_size_y;
        image_ratio_x = f128(input_size_x) / f128(output_size_x);
        image_ratio_y = f128(input_size_y) / f128(output_size_y);
        image_ratio_xy = f128(input_size_xy) / f128(output_size_xy);

        sample_count_x = 1;
        sample_count_y = 1;
        sample_count_xy = 1;
        f128 grid_scale_x = f128(sample_grid_width_percent) * 0.01_q;
        f128 grid_scale_y = f128(sample_grid_height_percent) * 0.01_q;
        sample_grid_scale_x = 1.0_q;
        sample_grid_scale_y = 1.0_q;
        sample_grid_offset_x = 0.5_q;
        sample_grid_offset_y = 0.5_q;

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

            if (input_size_x > output_size_x)
                sample_grid_shape_x = SAMPLE_GRID_SHAPE_Square;
            if (input_size_y > output_size_y)
                sample_grid_shape_y = SAMPLE_GRID_SHAPE_Square;
        }
        else
        {
            sample_grid_shape_x = SAMPLE_GRID_SHAPE_Square;
            sample_grid_shape_y = SAMPLE_GRID_SHAPE_Square;
        }

        f128 shape_scale_factor_x = (sample_grid_shape_x == SAMPLE_GRID_SHAPE_Circle) ? M_SQRT2q : 1._q;
        f128 shape_scale_factor_y = (sample_grid_shape_y == SAMPLE_GRID_SHAPE_Circle) ? M_SQRT2q : 1._q;

        // Shrinking
        if (input_size_x > output_size_x)
        {

            f128 scale_factor = f128 (grid_scale_x * image_ratio_x * shape_scale_factor_x) / 2.0_q;
            u64 additional_samples = u64 (scale_factor) * 2u;
            sample_count_x = 3u + additional_samples;
            sample_grid_scale_x = grid_scale_x * image_ratio_x * shape_scale_factor_x;

            sample_grid_offset_x = 0.5_q * image_ratio_x;
        }
        // Growing
        else if (input_size_x < output_size_x)
        {
            if ((grid_scale_x > 1.00001_q) || (sample_grid_shape_x != SAMPLE_GRID_SHAPE_Square))
            {
                f128 scale_factor = f128 (grid_scale_x * shape_scale_factor_x) * 0.5_q;
                u64 additional_samples = u64 (scale_factor) * 2u;
                sample_count_x = 3u + additional_samples;
                sample_grid_scale_x = (grid_scale_x * shape_scale_factor_x) - 1.0_q;
            }
            else
            {
                sample_count_x = 1u;
                sample_grid_scale_x = 1.0_q;
            }

            sample_grid_offset_x = 0.5_q * image_ratio_x;
        }
        // No change
        else
        {
            if ((grid_scale_x > 1.00001_q) || (sample_grid_shape_x != SAMPLE_GRID_SHAPE_Square))
            {
                f128 scale_factor = f128 (grid_scale_x * shape_scale_factor_x) * 0.5_q;
                u64 additional_samples = u64 (scale_factor) * 2u;
                sample_count_x = 3u + additional_samples;
                sample_grid_scale_x = (grid_scale_x * shape_scale_factor_x) - 1.0_q;
            }
            else
            {
                sample_count_x = 1u;
                sample_grid_scale_x = 1.0_q;
            }

            sample_grid_offset_x = 0.5_q;
        }

        // Shrinking
        if (input_size_y > output_size_y)
        {
            f128 scale_factor = f128 (grid_scale_y * image_ratio_y * shape_scale_factor_y) / 2.0_q;
            u64 additional_samples = u64 (scale_factor) * 2u;
            sample_count_y = 3u + additional_samples;
            sample_grid_scale_y = grid_scale_y * image_ratio_y * shape_scale_factor_y;

            sample_grid_offset_y = 0.5_q * image_ratio_y;
        }
        // Growing
        else if (input_size_y < output_size_y)
        {
            if ((grid_scale_y > 1.00001_q) || (sample_grid_shape_y != SAMPLE_GRID_SHAPE_Square))
            {
                f128 scale_factor = f128 (grid_scale_y * shape_scale_factor_y) * 0.5_q;
                u64 additional_samples = u64 (scale_factor) * 2u;
                sample_count_y = 3u + additional_samples;
                sample_grid_scale_y = (grid_scale_y * shape_scale_factor_y) - 1.0_q;
            }
            else
            {
                sample_count_y = 1u;
                sample_grid_scale_y = 1.0_q;
            }

            sample_grid_offset_y = 0.5_q * image_ratio_y;
        }
        // No change
        else
        {
            if ((grid_scale_y > 1.00001_q) || (sample_grid_shape_y != SAMPLE_GRID_SHAPE_Square))
            {
                f128 scale_factor = f128 (grid_scale_y * shape_scale_factor_y) * 0.5_q;
                u64 additional_samples = u64 (scale_factor) * 2u;
                sample_count_y = 3u + additional_samples;
                sample_grid_scale_y = (grid_scale_y * shape_scale_factor_y) - 1.0_q;
            }
            else
            {
                sample_count_y = 1u;
                sample_grid_scale_y = 1.0_q;
            }

            sample_grid_offset_y = 0.5_q;
        }

        if (sample_count_adjustment > 1.0004_q)
        {
            f128 s_c_xf = fmaxq(f128(sample_count_x) * sample_count_adjustment, 1.00001_q);
            f128 s_c_yf = fmaxq(f128(sample_count_y) * sample_count_adjustment, 1.00001_q);
            sample_count_x = (u64) to_intq(s_c_xf);
            sample_count_y = (u64) to_intq(s_c_yf);
        }

        sample_count_xy = sample_count_x * sample_count_y;
    }

    void ThpParams::GetSampleGridVectors(vector<SampleGridElement> *grid)
    {
        grid->clear();
        grid->resize(sample_count_xy, SampleGridElement());

        #pragma omp parallel for shared(grid) default(firstprivate)
        for (u64 index = 0u; index < sample_count_xy; index++)
        {
            u64 px = index % sample_count_x;
            u64 py = index / sample_count_x;

            f128 spos_x = 0._q;
            f128 spos_y = 0._q;

            if (sample_count_x > 1u)
                spos_x = 2._q * ((f128(px) / f128(sample_count_x - 1u)) - 0.5_q);
            if (sample_count_y > 1u)
                spos_y = 2._q * ((f128(py) / f128(sample_count_y - 1u)) - 0.5_q);

            if ((sample_grid_shape_x == SAMPLE_GRID_SHAPE_Square) && (sample_grid_shape_y == SAMPLE_GRID_SHAPE_Square))
            {
                #pragma omp atomic write
                grid->at(index).x = spos_x * 0.5_q;
                #pragma omp atomic write
                grid->at(index).y = spos_y * 0.5_q;
            }
            else
            {
                f128 hyp = sqrtq((spos_x * spos_x) + (spos_y * spos_y));
                f128 position_ellipse_x = 0._q;
                f128 position_ellipse_y = 0._q;
                if (hyp > 0._q)
                {
                    if (fabsq(spos_x) > 0._q)
                    {
                        position_ellipse_x = ((cosq(((hyp - spos_x) / fabsq(spos_x)) * SMP_GRID_ELLIPSE_COS_FACTOR) * M_SQRT2_m1q) + 1._q) * spos_x;
                    }
                    if (fabsq(spos_y) > 0._q)
                    {
                        position_ellipse_y = ((cosq(((hyp - spos_y) / fabsq(spos_y)) * SMP_GRID_ELLIPSE_COS_FACTOR) * M_SQRT2_m1q) + 1._q) * spos_y;
                    }
                }

                #pragma omp atomic write
                grid->at(index).x = (sample_grid_shape_x == SAMPLE_GRID_SHAPE_Square) ? spos_x * 0.5_q : position_ellipse_x * 0.5_q;
                #pragma omp atomic write
                grid->at(index).y = (sample_grid_shape_y == SAMPLE_GRID_SHAPE_Square) ? spos_y * 0.5_q : position_ellipse_y * 0.5_q;
            }

            if ((sample_grid_weighting < 0.000005_q) || (sample_count_xy < 2u))
            {
                #pragma omp atomic write
                grid->at(index).weight = 1.0_q;
            }
            else
            {
                f128 gx = 0.0_q, gy = 0.0_q;

                #pragma omp atomic read
                gx = grid->at(index).x;
                #pragma omp atomic read
                gy = grid->at(index).y;

                f128 hyp = sqrtq((gx * gx * 4.0_q) + (gy * gy * 4.0_q));
                f128 weight_factor = clamp01q(hyp / M_SQRT2q);
                f128 sample_weight_base = cosq(weight_factor * M_PI_2q);
                f128 sample_weight = lerpq(1._q, sample_weight_base, sample_grid_weighting);

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
    }

    void ThpParams::CalcNumberOfChunks()
    {
        sample_count_xy = max(sample_count_xy, 1uL);
        chunk_size_default = max(1024uL * chunk_size_kilo, 1024uL);
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
    }
};
