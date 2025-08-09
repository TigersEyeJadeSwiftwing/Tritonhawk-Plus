#include "components/thp_types.h"
#include "components/thp_resize.h"
#include "components/thp_log.h"
#include "components/thp_parameters.h"

#include "components/inline/thp_pixel_vector.inl"

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

extern ThpLog* Log;

namespace TritonhawkPlus
{
    void Thp_Resize_drawable_RGBA(ThpParams* params, GimpDrawable* draw_0, GimpDrawable* draw_1)
    {
        if ((!draw_0) || (!draw_1) || (!params)) return;
        u64 progress_steps = params->number_chunks;
        if (progress_steps < 1) return;

        u64 old_x = (u64)gimp_drawable_get_width (draw_0);
        u64 old_y = (u64)gimp_drawable_get_height (draw_0);
        u64 new_x = (u64)gimp_drawable_get_width (draw_1);
        u64 new_y = (u64)gimp_drawable_get_height (draw_1);
        u64 old_total = old_x * old_y;
        u64 new_total = new_x * new_y;
        if ((old_total < 1) || (new_total < 1)) return;

        params->input_size_x = old_x;
        params->input_size_y = old_y;
        params->input_size_xy = old_total;
        params->output_size_x = new_x;
        params->output_size_y = new_y;
        params->output_size_xy = new_total;
        params->CalcAll();

        int draw_index = params->draw_index;
        bool seamless_x = params->seamless_x;
        bool seamless_y = params->seamless_y;
        u64 chunk_size = params->chunk_size_pixels;
        f64 progress_start = params->progress_start;
        f64 progress_end = params->progress_end;
        f64 progress_increment = params->progress_increment;
        u64 samples_x = params->sample_count_x;
        u64 samples_y = params->sample_count_y;
        u64 samples_total = params->sample_count_xy;
        f128 sample_grid_width = params->sample_grid_scale_x;
        f128 sample_grid_height = params->sample_grid_scale_y;
        f128 sample_grid_offset_x = params->sample_grid_offset_x;
        f128 sample_grid_offset_y = params->sample_grid_offset_y;
        f128 sample_interpolation_x = params->sample_interpolation_x;
        f128 sample_interpolation_y = params->sample_interpolation_y;
        int sample_grid_shape = params->sample_grid_shape;

        vector<SampleGridElement> sample_data;
        params->GetSampleGridVectors(&sample_data);

        /*
        if ((new_x < old_x) && (new_y < old_y))
            sample_grid_shape = 0;
        */

        string process_text_base = g_strdup_printf (
            _(  "%s"
                "-     Current drawable: %i / %i" "\n"
                "-       Current old image drawable size: %I64u x %I64u pixels, %I64u total pixels" "\n"
                "-       Current new image drawable size: %I64u x %I64u pixels, %I64u total pixels" "\n"
                "-       Number of chunks: %I64u, Chunk size: %I64u pixels" "\n"
            ),
            params->info_string.c_str(),
            draw_index + 1, params->draw_count,
            old_x, old_y, old_total,
            new_x, new_y, new_total,
            progress_steps, chunk_size
        );

        vector<f64> old_pixelarray(old_total * 4, 0.0);
        vector<f64> new_pixelarray(new_total * 4, 0.0);

        // Read source drawable
        {
            const GeglRectangle* rect = gegl_rectangle_new((gint)0, (gint)0, (guint)old_x, (guint)old_y);
            const Babl* format = babl_format_with_space("RGBA double", NULL);
            double* pxl = new double[old_total * 4];
            GeglBuffer* buff = gimp_drawable_get_buffer(draw_0);
            gegl_buffer_get(buff, rect, 1.0, format, pxl, GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);
            old_pixelarray.assign(pxl, pxl + (old_total * 4));
            g_clear_object(&buff);
            delete[] pxl;
            delete rect;
        }

        u64 chunk_accum = 0;

        // Process Chunks
        #pragma omp parallel for \
            shared( \
                chunk_accum, process_text_base, old_pixelarray, new_pixelarray, Log \
            ) \
            firstprivate( \
                new_x, new_y, new_total, old_x, old_y, old_total, chunk_size, \
                samples_total, samples_x, samples_y, sample_grid_width, sample_grid_height, sample_grid_shape,\
                sample_grid_offset_x, sample_grid_offset_y, sample_interpolation_x, sample_interpolation_y, \
                progress_steps, progress_start, progress_end, progress_increment, \
                seamless_x, seamless_y \
            )
        for (u64 chunk_index = 0; chunk_index <= progress_steps; chunk_index++)
        {
            u64 pixel_start = chunk_index * chunk_size;
            u64 pixel_end = min(pixel_start + chunk_size, new_total);
            u64 pixel_total = new_total;

            if (pixel_start >= pixel_total) continue;

            #pragma omp atomic update
            chunk_accum++;

            if (omp_get_thread_num() == 0)
            {
                u64 progress_chunks = 0;
                #pragma omp atomic read
                progress_chunks = chunk_accum;
                f64 progress_current = fmin(100.0 * ( progress_start + (f64(progress_chunks) * progress_increment) ), 100.0);

                Log->SetString(g_strdup_printf (
                    _(  "%s"
                        "-     Processing pixels: %I64u - %I64u of %I64u" "\n"
                        "-       Chunks completed so far: %I64u / %I64u" "\n"
                        "-       Total progress (all drawables): %%%3.4lf"
                    ),
                    process_text_base.c_str(),
                    pixel_start, pixel_end, pixel_total,
                    progress_chunks, progress_steps,
                    progress_current)
                );
                Log->SetStringProgressBar( g_strdup_printf( _("Processing chunk %I64u of %I64u... %%%3.4lf complete..."), progress_chunks, progress_steps, progress_current ) );
                Log->progress_bar_fraction = progress_current;
                Log->AutoLog(true);
            }

            #pragma omp parallel for \
                shared( \
                    old_pixelarray, new_pixelarray \
                ) \
                firstprivate( \
                    new_x, new_y, old_x, old_y, \
                    samples_total, samples_x, samples_y, sample_grid_width, sample_grid_height, sample_grid_shape, \
                    sample_grid_offset_x, sample_grid_offset_y, sample_interpolation_x, sample_interpolation_y, \
                    seamless_x, seamless_y, \
                    pixel_start, pixel_end \
                )
            for (s64 pixel_index = pixel_start; pixel_index < pixel_end; pixel_index++)
            {
                s64 p1_x = pixel_index % new_x;
                s64 p1_y = pixel_index / new_x;
                f128 sample_grid_center_x = f128(old_x) * f128(p1_x) / f128(new_x);
                f128 sample_grid_center_y = f128(old_y) * f128(p1_y) / f128(new_y);
                // sample_grid_center_x += sample_grid_offset_x;
                // sample_grid_center_y += sample_grid_offset_y;
                // f128 sample_count = fmaxq(f128(samples_total), 1.0_q);
                f128 s_accum_r = 0.0_q, s_accum_g = 0.0_q, s_accum_b = 0.0_q, s_accum_a = 0.0_q, s_accum_weight = 0.0_q;

                #pragma omp parallel for collapse(2) \
                    shared( \
                        old_pixelarray, sample_data \
                    ) \
                    firstprivate( \
                        old_x, old_y, \
                        samples_total, samples_x, samples_y, sample_grid_width, sample_grid_height, sample_grid_shape, \
                        sample_interpolation_x, sample_interpolation_y, \
                        seamless_x, seamless_y, \
                        sample_grid_center_x, sample_grid_center_y \
                    ) \
                    reduction(+:s_accum_r, s_accum_g, s_accum_b, s_accum_a, s_accum_weight)
                for (s32 s_y = 0; s_y < samples_y; s_y++)
                {
                    #pragma clang loop vectorize(enable)
                    for (s32 s_x = 0; s_x < samples_x; s_x++)
                    {
                        s32 s_index = s_x + (s_y * samples_x);
                        f128 smp_grid_x = 0.0_q;
                        f128 smp_grid_y = 0.0_q;
                        f128 smp_weight = 1.0_q;

                        #pragma omp atomic read
                        smp_grid_x = sample_data.at(s_index).x;
                        #pragma omp atomic read
                        smp_grid_y = sample_data.at(s_index).y;
                        #pragma omp atomic read
                        smp_weight = sample_data.at(s_index).weight;

                        f128 sample_position_x = sample_grid_center_x + smp_grid_x;
                        f128 sample_position_y = sample_grid_center_y + smp_grid_y;

                        s32 pos_x0 = 0, pos_x1 = 0, pos_y0 = 0, pos_y1 = 0;
                        f128 lerp_x = 0.0_q, lerp_y = 0.0_q;
                        f128 oxf = f128(old_x);
                        f128 oyf = f128(old_y);

                        if (sample_interpolation_x < 0.00001_q)
                        {
                            sample_position_x = fmodq(sample_position_x + oxf, oxf);
                            pos_x0 = to_intq(sample_position_x);
                            pos_x1 = pos_x0;
                            lerp_x = 0.0_q;
                        }
                        else if (seamless_x == true)
                        {
                            sample_position_x = fmodq(sample_position_x + oxf, oxf);
                            pos_x0 = to_intq(sample_position_x);
                            f128 offcenter = fmodq(sample_position_x, 1.0_q);
                            if (offcenter < 0.5_q)
                            {
                                pos_x1 = (old_x + pos_x0 - 1) % old_x;
                                lerp_x = 0.5_q - offcenter;
                            }
                            else
                            {
                                pos_x1 = (old_x + pos_x0 + 1) % old_x;
                                lerp_x = offcenter - 0.5_q;
                            }
                        }
                        else
                        {
                            sample_position_x = fminq(fmaxq(sample_position_x, 0.0_q), oxf);
                            if (sample_position_x < 0.5_q)
                            {
                                pos_x0 = 0;
                                pos_x1 = 0;
                                lerp_x = 0.0_q;
                            }
                            else if (sample_position_x > oxf - 0.5_q)
                            {
                                pos_x0 = old_x - 1;
                                pos_x1 = old_x - 1;
                                lerp_x = 0.0_q;
                            }
                            else
                            {
                                pos_x0 = to_intq(sample_position_x);
                                f128 offcenter = fmodq(sample_position_x, 1.0_q);

                                if (offcenter < 0.5_q)
                                {
                                    pos_x1 = min(max(pos_x0 - 1, 0), old_x - 1);
                                    lerp_x = 0.5_q - offcenter;
                                }
                                else
                                {
                                    pos_x1 = min(max(pos_x0 + 1, 0), old_x - 1);
                                    lerp_x = offcenter - 0.5_q;
                                }
                            }
                        }

                        if (sample_interpolation_y < 0.00001_q)
                        {
                            sample_position_y = fmodq(sample_position_y + oyf, oyf);
                            pos_y0 = to_intq(sample_position_y);
                            pos_y1 = pos_y0;
                            lerp_y = 0.0_q;
                        }
                        else if (seamless_y == true)
                        {
                            sample_position_y = fmodq(sample_position_y + oyf, oyf);
                            pos_y0 = to_intq(sample_position_y);
                            f128 offcenter = fmodq(sample_position_y, 1.0_q);
                            if (offcenter < 0.5_q)
                            {
                                pos_y1 = (old_y + pos_y0 - 1) % old_y;
                                lerp_y = 0.5_q - offcenter;
                            }
                            else
                            {
                                pos_y1 = (old_y + pos_y0 + 1) % old_y;
                                lerp_y = offcenter - 0.5_q;
                            }
                        }
                        else
                        {
                            sample_position_y = fminq(fmaxq(sample_position_y, 0.0_q), oyf);
                            if (sample_position_y < 0.5_q)
                            {
                                pos_y0 = 0;
                                pos_y1 = 0;
                                lerp_y = 0.0_q;
                            }
                            else if (sample_position_y > oyf - 0.5_q)
                            {
                                pos_y0 = old_y - 1;
                                pos_y1 = old_y - 1;
                                lerp_y = 0.0_q;
                            }
                            else
                            {
                                pos_y0 = to_intq(sample_position_y);
                                f128 offcenter = fmodq(sample_position_y, 1.0_q);

                                if (offcenter < 0.5_q)
                                {
                                    pos_y1 = min(max(pos_y0 - 1, 0), old_y - 1);
                                    lerp_y = 0.5_q - offcenter;
                                }
                                else
                                {
                                    pos_y1 = min(max(pos_y0 + 1, 0), old_y - 1);
                                    lerp_y = offcenter - 0.5_q;
                                }
                            }
                        }

                        if ((sample_interpolation_x > 1.0_q) && (pos_x0 != pos_x1))
                            lerp_x = lerp2expq(0.0_q, 1.0_q, lerp_x, sample_interpolation_x);
                        if ((sample_interpolation_y > 1.0_q) && (pos_y0 != pos_y1))
                            lerp_y = lerp2expq(0.0_q, 1.0_q, lerp_y, sample_interpolation_y);

                        if ((pos_x0 == pos_x1) && (pos_y0 == pos_y1))
                        {
                            u64 index_r0 = RGBA_red(old_x, pos_x0, pos_y0);
                            u64 index_g0 = RGBA_green(old_x, pos_x0, pos_y0);
                            u64 index_b0 = RGBA_blue(old_x, pos_x0, pos_y0);
                            u64 index_a0 = RGBA_alpha(old_x, pos_x0, pos_y0);

                            f64 col_r0 = 0.0, col_g0 = 0.0, col_b0 = 0.0, col_a0 = 0.0;

                            #pragma omp atomic read
                            col_r0 = old_pixelarray[index_r0];
                            #pragma omp atomic read
                            col_g0 = old_pixelarray[index_g0];
                            #pragma omp atomic read
                            col_b0 = old_pixelarray[index_b0];
                            #pragma omp atomic read
                            col_a0 = old_pixelarray[index_a0];

                            s_accum_r += f128(col_r0) * smp_weight;
                            s_accum_g += f128(col_g0) * smp_weight;
                            s_accum_b += f128(col_b0) * smp_weight;
                            s_accum_a += f128(col_a0) * smp_weight;
                        }
                        else if ((pos_x0 != pos_x1) && (pos_y0 == pos_y1))
                        {
                            u64 index_r0 = RGBA_red(old_x, pos_x0, pos_y0);
                            u64 index_g0 = RGBA_green(old_x, pos_x0, pos_y0);
                            u64 index_b0 = RGBA_blue(old_x, pos_x0, pos_y0);
                            u64 index_a0 = RGBA_alpha(old_x, pos_x0, pos_y0);

                            u64 index_r1 = RGBA_red(old_x, pos_x1, pos_y1);
                            u64 index_g1 = RGBA_green(old_x, pos_x1, pos_y1);
                            u64 index_b1 = RGBA_blue(old_x, pos_x1, pos_y1);
                            u64 index_a1 = RGBA_alpha(old_x, pos_x1, pos_y1);

                            f64 col_r0 = 0.0, col_g0 = 0.0, col_b0 = 0.0, col_a0 = 0.0;
                            f64 col_r1 = 0.0, col_g1 = 0.0, col_b1 = 0.0, col_a1 = 0.0;

                            #pragma omp atomic read
                            col_r0 = old_pixelarray[index_r0];
                            #pragma omp atomic read
                            col_g0 = old_pixelarray[index_g0];
                            #pragma omp atomic read
                            col_b0 = old_pixelarray[index_b0];
                            #pragma omp atomic read
                            col_a0 = old_pixelarray[index_a0];

                            #pragma omp atomic read
                            col_r1 = old_pixelarray[index_r1];
                            #pragma omp atomic read
                            col_g1 = old_pixelarray[index_g1];
                            #pragma omp atomic read
                            col_b1 = old_pixelarray[index_b1];
                            #pragma omp atomic read
                            col_a1 = old_pixelarray[index_a1];

                            s_accum_r += lerpq(f128(col_r0), f128(col_r1), lerp_x) * smp_weight;
                            s_accum_g += lerpq(f128(col_g0), f128(col_g1), lerp_x) * smp_weight;
                            s_accum_b += lerpq(f128(col_b0), f128(col_b1), lerp_x) * smp_weight;
                            s_accum_a += lerpq(f128(col_a0), f128(col_a1), lerp_x) * smp_weight;
                        }
                        else if ((pos_x0 == pos_x1) && (pos_y0 != pos_y1))
                        {
                            u64 index_r0 = RGBA_red(old_x, pos_x0, pos_y0);
                            u64 index_g0 = RGBA_green(old_x, pos_x0, pos_y0);
                            u64 index_b0 = RGBA_blue(old_x, pos_x0, pos_y0);
                            u64 index_a0 = RGBA_alpha(old_x, pos_x0, pos_y0);

                            u64 index_r1 = RGBA_red(old_x, pos_x1, pos_y1);
                            u64 index_g1 = RGBA_green(old_x, pos_x1, pos_y1);
                            u64 index_b1 = RGBA_blue(old_x, pos_x1, pos_y1);
                            u64 index_a1 = RGBA_alpha(old_x, pos_x1, pos_y1);

                            f64 col_r0 = 0.0, col_g0 = 0.0, col_b0 = 0.0, col_a0 = 0.0;
                            f64 col_r1 = 0.0, col_g1 = 0.0, col_b1 = 0.0, col_a1 = 0.0;

                            #pragma omp atomic read
                            col_r0 = old_pixelarray[index_r0];
                            #pragma omp atomic read
                            col_g0 = old_pixelarray[index_g0];
                            #pragma omp atomic read
                            col_b0 = old_pixelarray[index_b0];
                            #pragma omp atomic read
                            col_a0 = old_pixelarray[index_a0];

                            #pragma omp atomic read
                            col_r1 = old_pixelarray[index_r1];
                            #pragma omp atomic read
                            col_g1 = old_pixelarray[index_g1];
                            #pragma omp atomic read
                            col_b1 = old_pixelarray[index_b1];
                            #pragma omp atomic read
                            col_a1 = old_pixelarray[index_a1];

                            s_accum_r += lerpq(f128(col_r0), f128(col_r1), lerp_y) * smp_weight;
                            s_accum_g += lerpq(f128(col_g0), f128(col_g1), lerp_y) * smp_weight;
                            s_accum_b += lerpq(f128(col_b0), f128(col_b1), lerp_y) * smp_weight;
                            s_accum_a += lerpq(f128(col_a0), f128(col_a1), lerp_y) * smp_weight;
                        }
                        else
                        {
                            u64 index_r00 = RGBA_red(old_x, pos_x0, pos_y0);
                            u64 index_g00 = RGBA_green(old_x, pos_x0, pos_y0);
                            u64 index_b00 = RGBA_blue(old_x, pos_x0, pos_y0);
                            u64 index_a00 = RGBA_alpha(old_x, pos_x0, pos_y0);

                            u64 index_r10 = RGBA_red(old_x, pos_x1, pos_y0);
                            u64 index_g10 = RGBA_green(old_x, pos_x1, pos_y0);
                            u64 index_b10 = RGBA_blue(old_x, pos_x1, pos_y0);
                            u64 index_a10 = RGBA_alpha(old_x, pos_x1, pos_y0);

                            u64 index_r01 = RGBA_red(old_x, pos_x0, pos_y1);
                            u64 index_g01 = RGBA_green(old_x, pos_x0, pos_y1);
                            u64 index_b01 = RGBA_blue(old_x, pos_x0, pos_y1);
                            u64 index_a01 = RGBA_alpha(old_x, pos_x0, pos_y1);

                            u64 index_r11 = RGBA_red(old_x, pos_x1, pos_y1);
                            u64 index_g11 = RGBA_green(old_x, pos_x1, pos_y1);
                            u64 index_b11 = RGBA_blue(old_x, pos_x1, pos_y1);
                            u64 index_a11 = RGBA_alpha(old_x, pos_x1, pos_y1);

                            f64 col_r00 = 0.0, col_g00 = 0.0, col_b00 = 0.0, col_a00 = 0.0;
                            f64 col_r10 = 0.0, col_g10 = 0.0, col_b10 = 0.0, col_a10 = 0.0;
                            f64 col_r01 = 0.0, col_g01 = 0.0, col_b01 = 0.0, col_a01 = 0.0;
                            f64 col_r11 = 0.0, col_g11 = 0.0, col_b11 = 0.0, col_a11 = 0.0;

                            #pragma omp atomic read
                            col_r00 = old_pixelarray[index_r00];
                            #pragma omp atomic read
                            col_g00 = old_pixelarray[index_g00];
                            #pragma omp atomic read
                            col_b00 = old_pixelarray[index_b00];
                            #pragma omp atomic read
                            col_a00 = old_pixelarray[index_a00];

                            #pragma omp atomic read
                            col_r10 = old_pixelarray[index_r10];
                            #pragma omp atomic read
                            col_g10 = old_pixelarray[index_g10];
                            #pragma omp atomic read
                            col_b10 = old_pixelarray[index_b10];
                            #pragma omp atomic read
                            col_a10 = old_pixelarray[index_a10];

                            #pragma omp atomic read
                            col_r01 = old_pixelarray[index_r01];
                            #pragma omp atomic read
                            col_g01 = old_pixelarray[index_g01];
                            #pragma omp atomic read
                            col_b01 = old_pixelarray[index_b01];
                            #pragma omp atomic read
                            col_a01 = old_pixelarray[index_a01];

                            #pragma omp atomic read
                            col_r11 = old_pixelarray[index_r11];
                            #pragma omp atomic read
                            col_g11 = old_pixelarray[index_g11];
                            #pragma omp atomic read
                            col_b11 = old_pixelarray[index_b11];
                            #pragma omp atomic read
                            col_a11 = old_pixelarray[index_a11];

                            f128 mix_r0 = lerpq(f128(col_r00), f128(col_r10), lerp_x);
                            f128 mix_g0 = lerpq(f128(col_g00), f128(col_g10), lerp_x);
                            f128 mix_b0 = lerpq(f128(col_b00), f128(col_b10), lerp_x);
                            f128 mix_a0 = lerpq(f128(col_a00), f128(col_a10), lerp_x);

                            f128 mix_r1 = lerpq(f128(col_r01), f128(col_r11), lerp_x);
                            f128 mix_g1 = lerpq(f128(col_g01), f128(col_g11), lerp_x);
                            f128 mix_b1 = lerpq(f128(col_b01), f128(col_b11), lerp_x);
                            f128 mix_a1 = lerpq(f128(col_a01), f128(col_a11), lerp_x);

                            s_accum_r += lerpq(mix_r0, mix_r1, lerp_y) * smp_weight;
                            s_accum_g += lerpq(mix_g0, mix_g1, lerp_y) * smp_weight;
                            s_accum_b += lerpq(mix_b0, mix_b1, lerp_y) * smp_weight;
                            s_accum_a += lerpq(mix_a0, mix_a1, lerp_y) * smp_weight;
                        }

                        s_accum_weight += smp_weight;
                    }
                } // END nested OpenMP-enabled for(s_y = 0 - samples_y), for(s_x = 0 - samples_x)

                if (s_accum_weight > 0.0_q)
                {
                    s_accum_r /= s_accum_weight;
                    s_accum_g /= s_accum_weight;
                    s_accum_b /= s_accum_weight;
                    s_accum_a /= s_accum_weight;
                }

                {
                    u64 index_r = RGBA_red(new_x, p1_x, p1_y);
                    u64 index_g = RGBA_green(new_x, p1_x, p1_y);
                    u64 index_b = RGBA_blue(new_x, p1_x, p1_y);
                    u64 index_a = RGBA_alpha(new_x, p1_x, p1_y);

                    #pragma omp atomic write
                    new_pixelarray[index_r] = f64(s_accum_r);
                    #pragma omp atomic write
                    new_pixelarray[index_g] = f64(s_accum_g);
                    #pragma omp atomic write
                    new_pixelarray[index_b] = f64(s_accum_b);
                    #pragma omp atomic write
                    new_pixelarray[index_a] = f64(s_accum_a);
                }

                /*
                new_pixelarray[RGBA_red(new_x, p1_x, p1_y)] = f64(s_accum_r);
                new_pixelarray[RGBA_green(new_x, p1_x, p1_y)] = f64(s_accum_g);
                new_pixelarray[RGBA_blue(new_x, p1_x, p1_y)] = f64(s_accum_b);
                new_pixelarray[RGBA_alpha(new_x, p1_x, p1_y)] = f64(s_accum_a);
                */

            } // END OpenMP-enabled for (pixel_index = pixel_start - pixel_end)

        } // END OpenMP-enabled for (int chunk_index = 0; chunk_index <= progress_steps; chunk_index++)

        // Write output drawable
        {
            const GeglRectangle* rect = gegl_rectangle_new((gint)0, (gint)0, (guint)new_x, (guint)new_y);
            const Babl* format = babl_format_with_space("RGBA double", NULL);
            double* pxl = new double[new_total * 4];
            std::copy(new_pixelarray.begin(), new_pixelarray.end(), pxl);
            GeglBuffer* buff = gimp_drawable_get_buffer(draw_1);
            gegl_buffer_set(buff, rect, 0, format, pxl, GEGL_AUTO_ROWSTRIDE);
            gegl_buffer_flush(buff);
            g_clear_object(&buff);
            delete[] pxl;
            delete rect;
        }
    } // END void Thp_Resize_drawable_RGBA()

    void Thp_Resize_drawable_RGB(ThpParams* params, GimpDrawable* draw_0, GimpDrawable* draw_1)
    {
        if ((!draw_0) || (!draw_1) || (!params)) return;
        const int progress_steps = params->number_chunks;
        if (progress_steps < 1) return;

        const int old_x = (int)gimp_drawable_get_width (draw_0);
        const int old_y = (int)gimp_drawable_get_height (draw_0);
        const int new_x = (int)gimp_drawable_get_width (draw_1);
        const int new_y = (int)gimp_drawable_get_height (draw_1);
        const int old_total = old_x * old_y;
        const int new_total = new_x * new_y;
        if ((old_total < 1) || (new_total < 1)) return;

        params->input_size_x = old_x;
        params->input_size_y = old_y;
        params->input_size_xy = old_total;
        params->output_size_x = new_x;
        params->output_size_y = new_y;
        params->output_size_xy = new_total;
        params->CalcAll();

        const int draw_index = params->draw_index;
        const bool seamless_x = params->seamless_x;
        const bool seamless_y = params->seamless_y;
        const int chunk_size = params->chunk_size_pixels;
        const f64 progress_start = params->progress_start;
        const f64 progress_end = params->progress_end;
        const f64 progress_increment = params->progress_increment;
        const int samples_x = params->sample_count_x;
        const int samples_y = params->sample_count_y;
        const int samples_total = params->sample_count_xy;
        const f128 sample_grid_width = params->sample_grid_scale_x;
        const f128 sample_grid_height = params->sample_grid_scale_y;
        const f128 sample_grid_offset_x = params->sample_grid_offset_x;
        const f128 sample_grid_offset_y = params->sample_grid_offset_y;
        const f128 sample_interpolation_x = params->sample_interpolation_x;
        const f128 sample_interpolation_y = params->sample_interpolation_y;

        string process_text_base = g_strdup_printf (
            _(  "%s"
                "-     Current drawable: %i / %i" "\n"
                "-       Current old image drawable size: %i x %i pixels, %i total pixels" "\n"
                "-       Current new image drawable size: %i x %i pixels, %i total pixels" "\n"
                "-       Number of chunks: %i, Chunk size: %i pixels" "\n"
            ),
            params->info_string.c_str(),
            draw_index + 1, params->draw_count,
            old_x, old_y, old_total,
            new_x, new_y, new_total,
            progress_steps, chunk_size
        );

        vector<f64> old_pixelarray(old_total * 3, 0.0);
        vector<f64> new_pixelarray(new_total * 3, 0.0);

        // Read source drawable
        {
            const GeglRectangle* rect = gegl_rectangle_new((gint)0, (gint)0, (guint)old_x, (guint)old_y);
            const Babl* format = babl_format_with_space("RGB double", NULL);
            double* pxl = new double[old_total * 3];
            GeglBuffer* buff = gimp_drawable_get_buffer(draw_0);
            gegl_buffer_get(buff, rect, 1.0, format, pxl, GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);
            old_pixelarray.assign(pxl, pxl + (old_total * 3));
            g_clear_object(&buff);
            delete[] pxl;
            delete rect;
        }

        int chunk_accum = 0;

        // Process Chunks
        #pragma omp parallel for \
            shared( \
                chunk_accum, process_text_base, old_pixelarray, new_pixelarray, Log \
            ) \
            firstprivate( \
                new_x, new_y, new_total, old_x, old_y, old_total, chunk_size, \
                samples_total, samples_x, samples_y, sample_grid_width, sample_grid_height, \
                sample_grid_offset_x, sample_grid_offset_y, sample_interpolation_x, sample_interpolation_y, \
                progress_steps, progress_start, progress_end, progress_increment, \
                seamless_x, seamless_y \
            )
        for (int chunk_index = 0; chunk_index <= progress_steps; chunk_index++)
        {
            int pixel_start = chunk_index * chunk_size;
            int pixel_end = min(pixel_start + chunk_size, new_total);
            int pixel_total = new_total;

            if (pixel_start >= pixel_total) continue;

            #pragma omp atomic
            chunk_accum += 1;

            if (omp_get_thread_num() == 0)
            {
                int progress_chunks = 0;
                #pragma omp atomic read
                progress_chunks = chunk_accum;
                f64 progress_current = fmin(100.0 * ( progress_start + (f64(progress_chunks) * progress_increment) ), 100.0);

                Log->SetString(g_strdup_printf (
                    _(  "%s"
                        "-     Processing pixels: %i - %i of %i" "\n"
                        "-       Chunks completed so far: %i / %i" "\n"
                        "-       Total progress (all drawables): %%%3.2lf"
                    ),
                    process_text_base.c_str(),
                    pixel_start, pixel_end, new_total,
                    progress_chunks, progress_steps,
                    progress_current)
                );
                Log->SetStringProgressBar( g_strdup_printf( _("Processing chunk %i of %i... %%%3.2lf complete..."), progress_chunks, progress_steps, progress_current ) );
                Log->progress_bar_fraction = progress_current;
                Log->AutoLog(true);
            }

            #pragma omp parallel for \
                shared( \
                    old_pixelarray, new_pixelarray \
                ) \
                firstprivate( \
                    new_x, new_y, old_x, old_y, \
                    samples_total, samples_x, samples_y, sample_grid_width, sample_grid_height, \
                    sample_grid_offset_x, sample_grid_offset_y, sample_interpolation_x, sample_interpolation_y, \
                    seamless_x, seamless_y, \
                    pixel_start, pixel_end \
                )
            for (int pixel_index = pixel_start; pixel_index < pixel_end; pixel_index++)
            {
                int p1_x = pixel_index % new_x;
                int p1_y = pixel_index / new_x;
                f128 sample_grid_center_x = f128(old_x) * f128(p1_x) / f128(new_x);
                f128 sample_grid_center_y = f128(old_y) * f128(p1_y) / f128(new_y);
                sample_grid_center_x += sample_grid_offset_x;
                sample_grid_center_y += sample_grid_offset_y;
                f128 sample_count = fmaxq(f128(samples_total), 1.0_q);
                f128 s_accum_r = 0.0_q, s_accum_g = 0.0_q, s_accum_b = 0.0_q;

                #pragma omp parallel for collapse(2) \
                    shared( \
                        old_pixelarray \
                    ) \
                    firstprivate( \
                        old_x, old_y, \
                        samples_x, samples_y, sample_grid_width, sample_grid_height, \
                        sample_interpolation_x, sample_interpolation_y, \
                        seamless_x, seamless_y, \
                        sample_grid_center_x, sample_grid_center_y \
                    ) \
                    reduction(+:s_accum_r, s_accum_g, s_accum_b)
                for (int s_y = 0; s_y < samples_y; s_y++)
                {
                    for (int s_x = 0; s_x < samples_x; s_x++)
                    {
                        f128 sample_position_x = sample_grid_center_x;
                        f128 sample_position_y = sample_grid_center_y;
                        if (samples_x > 1)
                            sample_position_x += (sample_grid_width * f128(s_x) / f128(samples_x - 1)) - (sample_grid_width * 0.5_q);
                        if (samples_y > 1)
                            sample_position_y += (sample_grid_height * f128(s_y) / f128(samples_y - 1)) - (sample_grid_height * 0.5_q);

                        int pos_x0 = 0, pos_x1 = 0, pos_y0 = 0, pos_y1 = 0;
                        f128 lerp_x = 0.0_q, lerp_y = 0.0_q;
                        f128 oxf = f128(old_x);
                        f128 oyf = f128(old_y);

                        if (sample_interpolation_x < 0.00001_q)
                        {
                            sample_position_x = fmodq(sample_position_x + oxf, oxf);
                            pos_x0 = to_intq(sample_position_x);
                            pos_x1 = pos_x0;
                            lerp_x = 0.0_q;
                        }
                        else if (seamless_x == true)
                        {
                            sample_position_x = fmodq(sample_position_x + oxf, oxf);
                            pos_x0 = to_intq(sample_position_x);
                            f128 offcenter = fmodq(sample_position_x, 1.0_q);
                            if (offcenter < 0.5_q)
                            {
                                pos_x1 = (old_x + pos_x0 - 1) % old_x;
                                lerp_x = 0.5_q - offcenter;
                            }
                            else
                            {
                                pos_x1 = (old_x + pos_x0 + 1) % old_x;
                                lerp_x = offcenter - 0.5_q;
                            }
                        }
                        else
                        {
                            sample_position_x = fminq(fmaxq(sample_position_x, 0.0_q), oxf);
                            if (sample_position_x < 0.5_q)
                            {
                                pos_x0 = 0;
                                pos_x1 = 0;
                                lerp_x = 0.0_q;
                            }
                            else if (sample_position_x > oxf - 0.5_q)
                            {
                                pos_x0 = old_x - 1;
                                pos_x1 = old_x - 1;
                                lerp_x = 0.0_q;
                            }
                            else
                            {
                                pos_x0 = to_intq(sample_position_x);
                                f128 offcenter = fmodq(sample_position_x, 1.0_q);

                                if (offcenter < 0.5_q)
                                {
                                    pos_x1 = min(max(pos_x0 - 1, 0), old_x - 1);
                                    lerp_x = 0.5_q - offcenter;
                                }
                                else
                                {
                                    pos_x1 = min(max(pos_x0 + 1, 0), old_x - 1);
                                    lerp_x = offcenter - 0.5_q;
                                }
                            }
                        }

                        if (sample_interpolation_y < 0.00001_q)
                        {
                            sample_position_y = fmodq(sample_position_y + oyf, oyf);
                            pos_y0 = to_intq(sample_position_y);
                            pos_y1 = pos_y0;
                            lerp_y = 0.0_q;
                        }
                        else if (seamless_y == true)
                        {
                            sample_position_y = fmodq(sample_position_y + oyf, oyf);
                            pos_y0 = to_intq(sample_position_y);
                            f128 offcenter = fmodq(sample_position_y, 1.0_q);
                            if (offcenter < 0.5_q)
                            {
                                pos_y1 = (old_y + pos_y0 - 1) % old_y;
                                lerp_y = 0.5_q - offcenter;
                            }
                            else
                            {
                                pos_y1 = (old_y + pos_y0 + 1) % old_y;
                                lerp_y = offcenter - 0.5_q;
                            }
                        }
                        else
                        {
                            sample_position_y = fminq(fmaxq(sample_position_y, 0.0_q), oyf);
                            if (sample_position_y < 0.5_q)
                            {
                                pos_y0 = 0;
                                pos_y1 = 0;
                                lerp_y = 0.0_q;
                            }
                            else if (sample_position_y > oyf - 0.5_q)
                            {
                                pos_y0 = old_y - 1;
                                pos_y1 = old_y - 1;
                                lerp_y = 0.0_q;
                            }
                            else
                            {
                                pos_y0 = to_intq(sample_position_y);
                                f128 offcenter = fmodq(sample_position_y, 1.0_q);

                                if (offcenter < 0.5_q)
                                {
                                    pos_y1 = min(max(pos_y0 - 1, 0), old_y - 1);
                                    lerp_y = 0.5_q - offcenter;
                                }
                                else
                                {
                                    pos_y1 = min(max(pos_y0 + 1, 0), old_y - 1);
                                    lerp_y = offcenter - 0.5_q;
                                }
                            }
                        }

                        if ((sample_interpolation_x > 1.0_q) && (pos_x0 != pos_x1))
                            lerp_x = lerp2expq(0.0_q, 1.0_q, lerp_x, sample_interpolation_x);
                        if ((sample_interpolation_y > 1.0_q) && (pos_y0 != pos_y1))
                            lerp_y = lerp2expq(0.0_q, 1.0_q, lerp_y, sample_interpolation_y);

                        if ((pos_x0 == pos_x1) && (pos_y0 == pos_y1))
                        {
                            s_accum_r += f128(old_pixelarray[RGB_red(old_x, pos_x0, pos_y0)]);
                            s_accum_g += f128(old_pixelarray[RGB_green(old_x, pos_x0, pos_y0)]);
                            s_accum_b += f128(old_pixelarray[RGB_blue(old_x, pos_x0, pos_y0)]);
                        }
                        else if ((pos_x0 != pos_x1) && (pos_y0 == pos_y1))
                        {
                            s_accum_r += lerpq(f128(old_pixelarray[RGB_red(old_x, pos_x0, pos_y0)]), f128(old_pixelarray[RGB_red(old_x, pos_x1, pos_y0)]), lerp_x);
                            s_accum_g += lerpq(f128(old_pixelarray[RGB_green(old_x, pos_x0, pos_y0)]), f128(old_pixelarray[RGB_green(old_x, pos_x1, pos_y0)]), lerp_x);
                            s_accum_b += lerpq(f128(old_pixelarray[RGB_blue(old_x, pos_x0, pos_y0)]), f128(old_pixelarray[RGB_blue(old_x, pos_x1, pos_y0)]), lerp_x);
                        }
                        else if ((pos_x0 == pos_x1) && (pos_y0 != pos_y1))
                        {
                            s_accum_r += lerpq(f128(old_pixelarray[RGB_red(old_x, pos_x0, pos_y0)]), f128(old_pixelarray[RGB_red(old_x, pos_x0, pos_y1)]), lerp_y);
                            s_accum_g += lerpq(f128(old_pixelarray[RGB_green(old_x, pos_x0, pos_y0)]), f128(old_pixelarray[RGB_green(old_x, pos_x0, pos_y1)]), lerp_y);
                            s_accum_b += lerpq(f128(old_pixelarray[RGB_blue(old_x, pos_x0, pos_y0)]), f128(old_pixelarray[RGB_blue(old_x, pos_x0, pos_y1)]), lerp_y);
                        }
                        else
                        {
                            f128 mix_r0 = lerpq(f128(old_pixelarray[RGB_red(old_x, pos_x0, pos_y0)]), f128(old_pixelarray[RGB_red(old_x, pos_x1, pos_y0)]), lerp_x);
                            f128 mix_g0 = lerpq(f128(old_pixelarray[RGB_green(old_x, pos_x0, pos_y0)]), f128(old_pixelarray[RGB_green(old_x, pos_x1, pos_y0)]), lerp_x);
                            f128 mix_b0 = lerpq(f128(old_pixelarray[RGB_blue(old_x, pos_x0, pos_y0)]), f128(old_pixelarray[RGB_blue(old_x, pos_x1, pos_y0)]), lerp_x);

                            f128 mix_r1 = lerpq(f128(old_pixelarray[RGB_red(old_x, pos_x0, pos_y1)]), f128(old_pixelarray[RGB_red(old_x, pos_x1, pos_y1)]), lerp_x);
                            f128 mix_g1 = lerpq(f128(old_pixelarray[RGB_green(old_x, pos_x0, pos_y1)]), f128(old_pixelarray[RGB_green(old_x, pos_x1, pos_y1)]), lerp_x);
                            f128 mix_b1 = lerpq(f128(old_pixelarray[RGB_blue(old_x, pos_x0, pos_y1)]), f128(old_pixelarray[RGB_blue(old_x, pos_x1, pos_y1)]), lerp_x);

                            s_accum_r += lerpq( mix_r0, mix_r1, lerp_y );
                            s_accum_g += lerpq( mix_g0, mix_g1, lerp_y );
                            s_accum_b += lerpq( mix_b0, mix_b1, lerp_y );
                        }

                    }
                } // END nested OpenMP-enabled for(s_y = 0 - samples_y), for(s_x = 0 - samples_x)

                s_accum_r /= sample_count;
                s_accum_g /= sample_count;
                s_accum_b /= sample_count;

                new_pixelarray[RGB_red(new_x, p1_x, p1_y)] = f64(s_accum_r);
                new_pixelarray[RGB_green(new_x, p1_x, p1_y)] = f64(s_accum_g);
                new_pixelarray[RGB_blue(new_x, p1_x, p1_y)] = f64(s_accum_b);

            } // END OpenMP-enabled for (pixel_index = pixel_start - pixel_end)

        } // END OpenMP-enabled for (int chunk_index = 0; chunk_index <= progress_steps; chunk_index++)

        // Write output drawable
        {
            const GeglRectangle* rect = gegl_rectangle_new((gint)0, (gint)0, (guint)new_x, (guint)new_y);
            const Babl* format = babl_format_with_space("RGB double", NULL);
            double* pxl = new double[new_total * 3];
            std::copy(new_pixelarray.begin(), new_pixelarray.end(), pxl);
            GeglBuffer* buff = gimp_drawable_get_buffer(draw_1);
            gegl_buffer_set(buff, rect, 0, format, pxl, GEGL_AUTO_ROWSTRIDE);
            gegl_buffer_flush(buff);
            g_clear_object(&buff);
            delete[] pxl;
            delete rect;
        }
    } // END void Thp_Resize_drawable_RGB()

} // END namespace TritonhawkPlus
