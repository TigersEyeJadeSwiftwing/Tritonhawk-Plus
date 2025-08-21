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
    void Thp_Resize_drawable_RGBA(ThpParams* Params, GimpDrawable* draw_0, GimpDrawable* draw_1)
    {
        if ((!draw_0) || (!draw_1) || (!Params)) return;
        u64 progress_steps = Params->number_chunks;
        if (progress_steps < 1) return;

        u64 old_x = (u64)gimp_drawable_get_width (draw_0);
        u64 old_y = (u64)gimp_drawable_get_height (draw_0);
        u64 new_x = (u64)gimp_drawable_get_width (draw_1);
        u64 new_y = (u64)gimp_drawable_get_height (draw_1);
        u64 old_total = old_x * old_y;
        u64 new_total = new_x * new_y;
        if ((old_total < 1uL) || (new_total < 1uL)) return;

        Params->input_size_x = old_x;
        Params->input_size_y = old_y;
        Params->input_size_xy = old_total;
        Params->output_size_x = new_x;
        Params->output_size_y = new_y;
        Params->output_size_xy = new_total;
        Params->CalcAll();

        s32 draw_index = Params->draw_index;
        bool seamless_x = Params->seamless_x;
        bool seamless_y = Params->seamless_y;
        u64 chunk_size = Params->chunk_size_pixels;
        f64 progress_start = Params->progress_start;
        f64 progress_end = Params->progress_end;
        f64 progress_increment = Params->progress_increment;
        u64 samples_x = Params->sample_count_x;
        u64 samples_y = Params->sample_count_y;
        u64 samples_total = Params->sample_count_xy;
        f128 sample_grid_width = Params->sample_grid_scale_x;
        f128 sample_grid_height = Params->sample_grid_scale_y;
        f128 sample_grid_offset_x = Params->sample_grid_offset_x;
        f128 sample_grid_offset_y = Params->sample_grid_offset_y;
        f128 sample_interpolation_x = Params->sample_interpolation_x;
        f128 sample_interpolation_y = Params->sample_interpolation_y;

        vector<SampleGridElement> sample_grid_data;
        Params->GetSampleGridVectors(&sample_grid_data);

        string process_text_base = "";
        if (Params->gui_enabled == true)
        {
            process_text_base = g_strdup_printf (
                _(
                    "Current drawable: %i / %i" "\n"
                    "Current old image drawable size:" "\n"
                    "   %I64u x %I64u pixels, %I64u total pixels" "\n"
                    "Current new image drawable size:" "\n"
                    "   %I64u x %I64u pixels, %I64u total pixels" "\n"
                    "Number of chunks: %I64u" "\n"
                    "Chunk size: %I64u pixels" "\n"
                ),
                draw_index + 1, Params->draw_count,
                old_x, old_y, old_total,
                new_x, new_y, new_total,
                progress_steps, chunk_size
            );
        }

        vector<f64> old_pixelarray(old_total * 4uL, 0.0);
        vector<f64> new_pixelarray(new_total * 4uL, 0.0);

        // Read source drawable
        {
            const GeglRectangle* rect = gegl_rectangle_new((gint)0, (gint)0, (guint)old_x, (guint)old_y);
            const Babl* format = babl_format_with_space("RGBA double", NULL);
            double* pxl = new double[old_total * 4uL];
            GeglBuffer* buff = gimp_drawable_get_buffer(draw_0);
            gegl_buffer_get(buff, rect, 1.0, format, pxl, GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);
            old_pixelarray.assign(pxl, pxl + (old_total * 4uL));
            g_clear_object(&buff);
            delete[] pxl;
            delete rect;
        }

        s32 oxs = s32(old_x);
        s32 oys = s32(old_y);
        f128 oxf = f128(old_x);
        f128 oyf = f128(old_y);

        u64 chunk_accum = 0uL;

        // Process Chunks
        #pragma omp parallel for \
            shared( \
                chunk_accum, process_text_base, old_pixelarray, new_pixelarray, Log \
            ) \
            firstprivate( \
                new_x, new_y, new_total, old_total, chunk_size, \
                samples_total, samples_x, samples_y, sample_grid_width, sample_grid_height,\
                sample_grid_offset_x, sample_grid_offset_y, sample_interpolation_x, sample_interpolation_y, \
                progress_steps, progress_start, progress_end, progress_increment, \
                seamless_x, seamless_y \
            )
        for (u64 chunk_index = 0u; chunk_index <= progress_steps; chunk_index++)
        {
            u64 pixel_start = chunk_index * chunk_size;
            u64 pixel_end = min(pixel_start + chunk_size, new_total);
            u64 pixel_total = new_total;

            if (pixel_start >= pixel_total) continue;

            #pragma omp atomic update
            chunk_accum++;

            if (omp_get_thread_num() == 0)
            {
                if (process_text_base.empty() == false)
                {
                    u64 progress_chunks = 0uL;
                    #pragma omp atomic read
                    progress_chunks = chunk_accum;
                    f64 progress_current = fmin(100.0 * ( progress_start + (f64(progress_chunks) * progress_increment) ), 100.0);

                    Log->Run1(g_strdup_printf (
                        _(  "%s"
                            "\n"
                            "Processing pixels:" "\n"
                            "   %I64u - %I64u of %I64u" "\n"
                            "Chunks completed so far:" "\n"
                            "   %I64u / %I64u" "\n"
                            "\n"
                            "Total progress (all drawables): %%%3.5lf"
                        ),
                        process_text_base.c_str(),
                        pixel_start, pixel_end, pixel_total,
                        progress_chunks, progress_steps,
                        progress_current)
                    );
                }
            }

            #pragma omp parallel for \
                shared( \
                    old_pixelarray, new_pixelarray \
                ) \
                firstprivate( \
                    new_x, new_y, oxs, oys, oxf, oyf, \
                    samples_total, samples_x, samples_y, sample_grid_width, sample_grid_height, \
                    sample_grid_offset_x, sample_grid_offset_y, sample_interpolation_x, sample_interpolation_y, \
                    seamless_x, seamless_y, \
                    pixel_start, pixel_end \
                )
            for (u64 pixel_index = pixel_start; pixel_index < pixel_end; pixel_index++)
            {
                u64 p1_x = pixel_index % new_x;
                u64 p1_y = pixel_index / new_x;
                f128 sample_grid_center_x = oxf * f128(p1_x) / f128(new_x);
                f128 sample_grid_center_y = oyf * f128(p1_y) / f128(new_y);
                f128 s_accum_r = 0.0_q, s_accum_g = 0.0_q, s_accum_b = 0.0_q, s_accum_a = 0.0_q, s_accum_weight = 0.0_q;

                #pragma omp parallel for \
                    shared( \
                        old_pixelarray, sample_grid_data \
                    ) \
                    firstprivate( \
                        oxs, oys, oxf, oyf, \
                        samples_total, samples_x, samples_y, sample_grid_width, sample_grid_height, \
                        sample_interpolation_x, sample_interpolation_y, \
                        seamless_x, seamless_y, \
                        sample_grid_center_x, sample_grid_center_y \
                    ) \
                    reduction(+:s_accum_r, s_accum_g, s_accum_b, s_accum_a, s_accum_weight)
                for (u64 s_xy = 0uLL; s_xy < samples_total; s_xy++)
                {
                    f128 smp_grid_x = 0.0_q,
                         smp_grid_y = 0.0_q,
                         smp_weight = 1.0_q;

                    #pragma omp atomic read
                    smp_grid_x = sample_grid_data.at(s_xy).x;
                    #pragma omp atomic read
                    smp_grid_y = sample_grid_data.at(s_xy).y;
                    #pragma omp atomic read
                    smp_weight = sample_grid_data.at(s_xy).weight;

                    f128 sample_position_x = sample_grid_center_x + smp_grid_x;
                    f128 sample_position_y = sample_grid_center_y + smp_grid_y;

                    s32 pos_x0 = 0, pos_x1 = 0, pos_y0 = 0, pos_y1 = 0;
                    f128 lerp_x = 0.0_q, lerp_y = 0.0_q;

                    // If the original image dimension is only one pixel, this should be short, simple, and quick.
                    if (oxs == 1)
                    {
                        sample_position_x = 0.5_q;
                        pos_x0 = 0;
                        pos_x1 = 0;
                        lerp_x = 0.0_q;
                    }
                    // If we're not doing interpolation, and instead doing "nearest neighbor", this should be simple, fast, and fairly straightforward.
                    else if (sample_interpolation_x < 0.00005_q)
                    {
                        sample_position_x = fmodq(sample_position_x + oxf, oxf);
                        pos_x0 = to_intq(sample_position_x);
                        pos_x1 = pos_x0;
                        lerp_x = 0.0_q;
                    }
                    // If we're wrapping seamlessly at the border, the sample position might loop around, and we interpolate.
                    else if (seamless_x == true)
                    {
                        sample_position_x = fmodq(sample_position_x + oxf, oxf);
                        pos_x0 = to_intq(sample_position_x);
                        f128 offcenter = fmodq(sample_position_x, 1.0_q);

                        if (offcenter < 0.5_q)
                        {
                            pos_x1 = (oxs + pos_x0 - 1) % oxs;
                            lerp_x = 0.5_q - offcenter;
                        }
                        else
                        {
                            pos_x1 = (oxs + pos_x0 + 1) % oxs;
                            lerp_x = offcenter - 0.5_q;
                        }
                    }
                    // If we clamp to the borders and not wrap around, here we still interpolate.
                    else
                    {
                        sample_position_x = clampq(sample_position_x, 0._q, oxf);

                        // If the sample is at the edge or close enough to it, clamp and simplify.
                        if (sample_position_x < 0.5_q)
                        {
                            pos_x0 = 0;
                            pos_x1 = 0;
                            lerp_x = 0.0_q;
                        }
                        // If the sample is at the edge or close enough to it, clamp and simplify.
                        else if (sample_position_x > oxf - 0.5_q)
                        {
                            pos_x0 = oxs - 1;
                            pos_x1 = oxs - 1;
                            lerp_x = 0.0_q;
                        }
                        else
                        {
                            pos_x0 = to_intq(sample_position_x);
                            f128 offcenter = fmodq(sample_position_x, 1.0_q);

                            if (offcenter < 0.5_q)
                            {
                                pos_x1 = clamp(pos_x0 - 1, 0, oxs - 1);
                                lerp_x = 0.5_q - offcenter;
                            }
                            else
                            {
                                pos_x1 = clamp(pos_x0 + 1, 0, oxs - 1);
                                lerp_x = offcenter - 0.5_q;
                            }
                        }
                    }

                    // If the original image dimension is only one pixel, this should be short, simple, and quick.
                    if (oys == 1)
                    {
                        sample_position_y = 0.5_q;
                        pos_y0 = 0;
                        pos_y1 = 0;
                        lerp_y = 0.0_q;
                    }
                    // If we're not doing interpolation, and instead doing "nearest neighbor", this should be simple, fast, and fairly straightforward.
                    else if (sample_interpolation_y < 0.00005_q)
                    {
                        sample_position_y = fmodq(sample_position_y + oyf, oyf);
                        pos_y0 = to_intq(sample_position_y);
                        pos_y1 = pos_y0;
                        lerp_y = 0.0_q;
                    }
                    // If we're wrapping seamlessly at the border, the sample position might loop around, and we interpolate.
                    else if (seamless_y == true)
                    {
                        sample_position_y = fmodq(sample_position_y + oyf, oyf);

                        pos_y0 = to_intq(sample_position_y);
                        f128 offcenter = fmodq(sample_position_y, 1.0_q);

                        if (offcenter < 0.5_q)
                        {
                            pos_y1 = (oys + pos_y0 - 1) % oys;
                            lerp_y = 0.5_q - offcenter;
                        }
                        else
                        {
                            pos_y1 = (oys + pos_y0 + 1) % oys;
                            lerp_y = offcenter - 0.5_q;
                        }
                    }
                    // If we clamp to the borders and not wrap around, here we still interpolate.
                    else
                    {
                        sample_position_y = clampq(sample_position_y, 0._q, oyf);

                        // If the sample is at the edge or close enough to it, clamp and simplify.
                        if (sample_position_y < 0.5_q)
                        {
                            pos_y0 = 0;
                            pos_y1 = 0;
                            lerp_y = 0.0_q;
                        }
                        // If the sample is at the edge or close enough to it, clamp and simplify.
                        else if (sample_position_y > oyf - 0.5_q)
                        {
                            pos_y0 = oys - 1;
                            pos_y1 = oys - 1;
                            lerp_y = 0.0_q;
                        }
                        else
                        {
                            pos_y0 = to_intq(sample_position_y);
                            f128 offcenter = fmodq(sample_position_y, 1.0_q);

                            if (offcenter < 0.5_q)
                            {
                                pos_y1 = clamp(pos_y0 - 1, 0, oys - 1);
                                lerp_y = 0.5_q - offcenter;
                            }
                            else
                            {
                                pos_y1 = clamp(pos_y0 + 1, 0, oys - 1);
                                lerp_y = offcenter - 0.5_q;
                            }
                        }
                    }

                    // If we're interpolating, and the paramaters for such are above 1.0q, then we are interpolating in a way that
                    //   isn't linear, such as quadratic, cubic, or somewhere in-between types, etc.
                    if ((sample_interpolation_x > 1.00005_q) && (pos_x0 != pos_x1))
                        lerp_x = lerp_exp_inq(0.0_q, 1.0_q, lerp_x, sample_interpolation_x);
                    if ((sample_interpolation_y > 1.00005_q) && (pos_y0 != pos_y1))
                        lerp_y = lerp_exp_inq(0.0_q, 1.0_q, lerp_y, sample_interpolation_y);

                    // If we don't have interpolation of any kind with x or y dimensions, adding the sample times it's weight to
                    //   the "accum" or accumulation variables is simple and fast.
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
                    // Here we only interpolate along one axis, but not the other.
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
                    // Here we only interpolate along one axis, but not the other.
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
                    // Here we have interpolation of some sort along both the x and y axis.
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

                } // END OpenMP-enabled for loop

                // Calculate the final color values for the output pixel, from all of the samples that were added together and weighted.
                if (s_accum_weight > 0.0_q)
                {
                    s_accum_r /= s_accum_weight;
                    s_accum_g /= s_accum_weight;
                    s_accum_b /= s_accum_weight;
                    s_accum_a /= s_accum_weight;
                }

                // Write the output pixel color data to the output buffer, 64-bits per color.
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

            } // END OpenMP-enabled for (pixel_index = pixel_start - pixel_end)

        } // END OpenMP-enabled for (int chunk_index = 0; chunk_index <= progress_steps; chunk_index++)

        // Write output drawable
        {
            const GeglRectangle* rect = gegl_rectangle_new((gint)0, (gint)0, (guint)new_x, (guint)new_y);
            const Babl* format = babl_format_with_space("RGBA double", NULL);
            double* pxl = new double[new_total * 4u];
            std::copy(new_pixelarray.begin(), new_pixelarray.end(), pxl);
            GeglBuffer* buff = gimp_drawable_get_buffer(draw_1);
            gegl_buffer_set(buff, rect, 0, format, pxl, GEGL_AUTO_ROWSTRIDE);
            gegl_buffer_flush(buff);
            g_clear_object(&buff);
            delete[] pxl;
            delete rect;
        }
    } // END void Thp_Resize_drawable_RGBA()

    void Thp_Resize_drawable_RGB(ThpParams* Params, GimpDrawable* draw_0, GimpDrawable* draw_1)
    {
        if ((!draw_0) || (!draw_1) || (!Params)) return;
        u64 progress_steps = Params->number_chunks;
        if (progress_steps < 1) return;

        u64 old_x = (u64)gimp_drawable_get_width (draw_0);
        u64 old_y = (u64)gimp_drawable_get_height (draw_0);
        u64 new_x = (u64)gimp_drawable_get_width (draw_1);
        u64 new_y = (u64)gimp_drawable_get_height (draw_1);
        u64 old_total = old_x * old_y;
        u64 new_total = new_x * new_y;
        if ((old_total < 1uL) || (new_total < 1uL)) return;

        Params->input_size_x = old_x;
        Params->input_size_y = old_y;
        Params->input_size_xy = old_total;
        Params->output_size_x = new_x;
        Params->output_size_y = new_y;
        Params->output_size_xy = new_total;
        Params->CalcAll();

        s32 draw_index = Params->draw_index;
        bool seamless_x = Params->seamless_x;
        bool seamless_y = Params->seamless_y;
        u64 chunk_size = Params->chunk_size_pixels;
        f64 progress_start = Params->progress_start;
        f64 progress_end = Params->progress_end;
        f64 progress_increment = Params->progress_increment;
        u64 samples_x = Params->sample_count_x;
        u64 samples_y = Params->sample_count_y;
        u64 samples_total = Params->sample_count_xy;
        f128 sample_grid_width = Params->sample_grid_scale_x;
        f128 sample_grid_height = Params->sample_grid_scale_y;
        f128 sample_grid_offset_x = Params->sample_grid_offset_x;
        f128 sample_grid_offset_y = Params->sample_grid_offset_y;
        f128 sample_interpolation_x = Params->sample_interpolation_x;
        f128 sample_interpolation_y = Params->sample_interpolation_y;

        vector<SampleGridElement> sample_grid_data;
        Params->GetSampleGridVectors(&sample_grid_data);

        string process_text_base = "";
        if (Params->gui_enabled == true)
        {
            process_text_base = g_strdup_printf (
                _(
                    "Current drawable: %i / %i" "\n"
                    "Current old image drawable size:" "\n"
                    "   %I64u x %I64u pixels, %I64u total pixels" "\n"
                    "Current new image drawable size:" "\n"
                    "   %I64u x %I64u pixels, %I64u total pixels" "\n"
                    "Number of chunks: %I64u" "\n"
                    "Chunk size: %I64u pixels" "\n"
                ),
                draw_index + 1, Params->draw_count,
                old_x, old_y, old_total,
                new_x, new_y, new_total,
                progress_steps, chunk_size
            );
        }

        vector<f64> old_pixelarray(old_total * 3uL, 0.0);
        vector<f64> new_pixelarray(new_total * 3uL, 0.0);

        // Read source drawable
        {
            const GeglRectangle* rect = gegl_rectangle_new((gint)0, (gint)0, (guint)old_x, (guint)old_y);
            const Babl* format = babl_format_with_space("RGB double", NULL);
            double* pxl = new double[old_total * 3uL];
            GeglBuffer* buff = gimp_drawable_get_buffer(draw_0);
            gegl_buffer_get(buff, rect, 1.0, format, pxl, GEGL_AUTO_ROWSTRIDE, GEGL_ABYSS_NONE);
            old_pixelarray.assign(pxl, pxl + (old_total * 3uL));
            g_clear_object(&buff);
            delete[] pxl;
            delete rect;
        }

        s32 oxs = s32(old_x);
        s32 oys = s32(old_y);
        f128 oxf = f128(old_x);
        f128 oyf = f128(old_y);

        u64 chunk_accum = 0uL;

        // Process Chunks
        #pragma omp parallel for \
            shared( \
                chunk_accum, process_text_base, old_pixelarray, new_pixelarray, Log \
            ) \
            firstprivate( \
                new_x, new_y, new_total, old_total, chunk_size, \
                samples_total, samples_x, samples_y, sample_grid_width, sample_grid_height, \
                sample_grid_offset_x, sample_grid_offset_y, sample_interpolation_x, sample_interpolation_y, \
                progress_steps, progress_start, progress_end, progress_increment, \
                seamless_x, seamless_y \
            )
        for (u64 chunk_index = 0u; chunk_index <= progress_steps; chunk_index++)
        {
            u64 pixel_start = chunk_index * chunk_size;
            u64 pixel_end = min(pixel_start + chunk_size, new_total);
            u64 pixel_total = new_total;

            if (pixel_start >= pixel_total) continue;

            #pragma omp atomic update
            chunk_accum++;

            if (omp_get_thread_num() == 0)
            {
                if (process_text_base.empty() == false)
                {
                    u64 progress_chunks = 0uL;
                    #pragma omp atomic read
                    progress_chunks = chunk_accum;
                    f64 progress_current = fmin(100.0 * ( progress_start + (f64(progress_chunks) * progress_increment) ), 100.0);

                    Log->Run1(g_strdup_printf (
                        _(  "%s"
                            "\n"
                            "Processing pixels:" "\n"
                            "   %I64u - %I64u of %I64u" "\n"
                            "Chunks completed so far:" "\n"
                            "   %I64u / %I64u" "\n"
                            "\n"
                            "Total progress (all drawables): %%%3.5lf"
                        ),
                        process_text_base.c_str(),
                        pixel_start, pixel_end, pixel_total,
                        progress_chunks, progress_steps,
                        progress_current)
                    );
                }
            }

            #pragma omp parallel for \
                shared( \
                    old_pixelarray, new_pixelarray \
                ) \
                firstprivate( \
                    new_x, new_y, oxs, oys, oxf, oyf, \
                    samples_total, samples_x, samples_y, sample_grid_width, sample_grid_height, \
                    sample_grid_offset_x, sample_grid_offset_y, sample_interpolation_x, sample_interpolation_y, \
                    seamless_x, seamless_y, \
                    pixel_start, pixel_end \
                )
            for (u64 pixel_index = pixel_start; pixel_index < pixel_end; pixel_index++)
            {
                u64 p1_x = pixel_index % new_x;
                u64 p1_y = pixel_index / new_x;
                f128 sample_grid_center_x = oxf * f128(p1_x) / f128(new_x);
                f128 sample_grid_center_y = oyf * f128(p1_y) / f128(new_y);
                f128 s_accum_r = 0.0_q, s_accum_g = 0.0_q, s_accum_b = 0.0_q, s_accum_weight = 0.0_q;

                #pragma omp parallel for \
                    shared( \
                        old_pixelarray, sample_grid_data \
                    ) \
                    firstprivate( \
                        oxs, oys, oxf, oyf, \
                        samples_total, samples_x, samples_y, sample_grid_width, sample_grid_height, \
                        sample_interpolation_x, sample_interpolation_y, \
                        seamless_x, seamless_y, \
                        sample_grid_center_x, sample_grid_center_y \
                    ) \
                    reduction(+:s_accum_r, s_accum_g, s_accum_b, s_accum_weight)
                for (u64 s_xy = 0uL; s_xy < samples_total; s_xy++)
                {
                    f128 smp_grid_x = 0.0_q,
                         smp_grid_y = 0.0_q,
                         smp_weight = 1.0_q;

                    #pragma omp atomic read
                    smp_grid_x = sample_grid_data.at(s_xy).x;
                    #pragma omp atomic read
                    smp_grid_y = sample_grid_data.at(s_xy).y;
                    #pragma omp atomic read
                    smp_weight = sample_grid_data.at(s_xy).weight;

                    f128 sample_position_x = sample_grid_center_x + smp_grid_x;
                    f128 sample_position_y = sample_grid_center_y + smp_grid_y;

                    s32 pos_x0 = 0, pos_x1 = 0, pos_y0 = 0, pos_y1 = 0;
                    f128 lerp_x = 0.0_q, lerp_y = 0.0_q;

                    // If the original image dimension is only one pixel, this should be short, simple, and quick.
                    if (oxs == 1)
                    {
                        sample_position_x = 0.5_q;
                        pos_x0 = 0;
                        pos_x1 = 0;
                        lerp_x = 0.0_q;
                    }
                    // If we're not doing interpolation, and instead doing "nearest neighbor", this should be simple, fast, and fairly straightforward.
                    else if (sample_interpolation_x < 0.00001_q)
                    {
                        sample_position_x = fmodq(sample_position_x + oxf, oxf);
                        pos_x0 = to_intq(sample_position_x);
                        pos_x1 = pos_x0;
                        lerp_x = 0.0_q;
                    }
                    // If we're wrapping seamlessly at the border, the sample position might loop around, and we interpolate.
                    else if (seamless_x == true)
                    {
                        sample_position_x = fmodq(sample_position_x + oxf, oxf);
                        pos_x0 = to_intq(sample_position_x);
                        f128 offcenter = fmodq(sample_position_x, 1.0_q);

                        if (offcenter < 0.5_q)
                        {
                            pos_x1 = (oxs + pos_x0 - 1) % oxs;
                            lerp_x = 0.5_q - offcenter;
                        }
                        else
                        {
                            pos_x1 = (oxs + pos_x0 + 1) % oxs;
                            lerp_x = offcenter - 0.5_q;
                        }
                    }
                    // If we clamp to the borders and not wrap around, here we still interpolate.
                    else
                    {
                        sample_position_x = clampq(sample_position_x, 0._q, oxf);

                        // If the sample is at the edge or close enough to it, clamp and simplify.
                        if (sample_position_x < 0.5_q)
                        {
                            pos_x0 = 0;
                            pos_x1 = 0;
                            lerp_x = 0.0_q;
                        }
                        // If the sample is at the edge or close enough to it, clamp and simplify.
                        else if (sample_position_x > oxf - 0.5_q)
                        {
                            pos_x0 = oxs - 1;
                            pos_x1 = oxs - 1;
                            lerp_x = 0.0_q;
                        }
                        else
                        {
                            pos_x0 = to_intq(sample_position_x);
                            f128 offcenter = fmodq(sample_position_x, 1.0_q);

                            if (offcenter < 0.5_q)
                            {
                                pos_x1 = clamp(pos_x0 - 1, 0, oxs - 1);
                                lerp_x = 0.5_q - offcenter;
                            }
                            else
                            {
                                pos_x1 = clamp(pos_x0 + 1, 0, oxs - 1);
                                lerp_x = offcenter - 0.5_q;
                            }
                        }
                    }

                    // If the original image dimension is only one pixel, this should be short, simple, and quick.
                    if (oys == 1)
                    {
                        sample_position_y = 0.5_q;
                        pos_y0 = 0;
                        pos_y1 = 0;
                        lerp_y = 0.0_q;
                    }
                    // If we're not doing interpolation, and instead doing "nearest neighbor", this should be simple, fast, and fairly straightforward.
                    else if (sample_interpolation_y < 0.00001_q)
                    {
                        sample_position_y = fmodq(sample_position_y + oyf, oyf);
                        pos_y0 = to_intq(sample_position_y);
                        pos_y1 = pos_y0;
                        lerp_y = 0.0_q;
                    }
                    // If we're wrapping seamlessly at the border, the sample position might loop around, and we interpolate.
                    else if (seamless_y == true)
                    {
                        sample_position_y = fmodq(sample_position_y + oyf, oyf);

                        pos_y0 = to_intq(sample_position_y);
                        f128 offcenter = fmodq(sample_position_y, 1.0_q);

                        if (offcenter < 0.5_q)
                        {
                            pos_y1 = (oys + pos_y0 - 1) % oys;
                            lerp_y = 0.5_q - offcenter;
                        }
                        else
                        {
                            pos_y1 = (oys + pos_y0 + 1) % oys;
                            lerp_y = offcenter - 0.5_q;
                        }
                    }
                    // If we clamp to the borders and not wrap around, here we still interpolate.
                    else
                    {
                        sample_position_y = clampq(sample_position_y, 0._q, oyf);

                        // If the sample is at the edge or close enough to it, clamp and simplify.
                        if (sample_position_y < 0.5_q)
                        {
                            pos_y0 = 0;
                            pos_y1 = 0;
                            lerp_y = 0.0_q;
                        }
                        // If the sample is at the edge or close enough to it, clamp and simplify.
                        else if (sample_position_y > oyf - 0.5_q)
                        {
                            pos_y0 = oys - 1;
                            pos_y1 = oys - 1;
                            lerp_y = 0.0_q;
                        }
                        else
                        {
                            pos_y0 = to_intq(sample_position_y);
                            f128 offcenter = fmodq(sample_position_y, 1.0_q);

                            if (offcenter < 0.5_q)
                            {
                                pos_y1 = clamp(pos_y0 - 1, 0, oys - 1);
                                lerp_y = 0.5_q - offcenter;
                            }
                            else
                            {
                                pos_y1 = clamp(pos_y0 + 1, 0, oys - 1);
                                lerp_y = offcenter - 0.5_q;
                            }
                        }
                    }

                    // If we're interpolating, and the paramaters for such are above 1.0q, then we are interpolating in a way that
                    //   isn't linear, such as quadratic, cubic, or somewhere in-between types, etc.
                    if ((sample_interpolation_x > 1.00005_q) && (pos_x0 != pos_x1))
                        lerp_x = lerp_exp_inq(0.0_q, 1.0_q, lerp_x, sample_interpolation_x);
                    if ((sample_interpolation_y > 1.00005_q) && (pos_y0 != pos_y1))
                        lerp_y = lerp_exp_inq(0.0_q, 1.0_q, lerp_y, sample_interpolation_y);

                    // If we don't have interpolation of any kind with x or y dimensions, adding the sample times it's weight to
                    //   the "accum" or accumulation variables is simple and fast.
                    if ((pos_x0 == pos_x1) && (pos_y0 == pos_y1))
                    {
                        u64 index_r0 = RGB_red(old_x, pos_x0, pos_y0);
                        u64 index_g0 = RGB_green(old_x, pos_x0, pos_y0);
                        u64 index_b0 = RGB_blue(old_x, pos_x0, pos_y0);

                        f64 col_r0 = 0.0, col_g0 = 0.0, col_b0 = 0.0;

                        #pragma omp atomic read
                        col_r0 = old_pixelarray[index_r0];
                        #pragma omp atomic read
                        col_g0 = old_pixelarray[index_g0];
                        #pragma omp atomic read
                        col_b0 = old_pixelarray[index_b0];

                        s_accum_r += f128(col_r0) * smp_weight;
                        s_accum_g += f128(col_g0) * smp_weight;
                        s_accum_b += f128(col_b0) * smp_weight;
                    }
                    // Here we only interpolate along one axis, but not the other.
                    else if ((pos_x0 != pos_x1) && (pos_y0 == pos_y1))
                    {
                        u64 index_r0 = RGB_red(old_x, pos_x0, pos_y0);
                        u64 index_g0 = RGB_green(old_x, pos_x0, pos_y0);
                        u64 index_b0 = RGB_blue(old_x, pos_x0, pos_y0);

                        u64 index_r1 = RGB_red(old_x, pos_x1, pos_y1);
                        u64 index_g1 = RGB_green(old_x, pos_x1, pos_y1);
                        u64 index_b1 = RGB_blue(old_x, pos_x1, pos_y1);

                        f64 col_r0 = 0.0, col_g0 = 0.0, col_b0 = 0.0;
                        f64 col_r1 = 0.0, col_g1 = 0.0, col_b1 = 0.0;

                        #pragma omp atomic read
                        col_r0 = old_pixelarray[index_r0];
                        #pragma omp atomic read
                        col_g0 = old_pixelarray[index_g0];
                        #pragma omp atomic read
                        col_b0 = old_pixelarray[index_b0];

                        #pragma omp atomic read
                        col_r1 = old_pixelarray[index_r1];
                        #pragma omp atomic read
                        col_g1 = old_pixelarray[index_g1];
                        #pragma omp atomic read
                        col_b1 = old_pixelarray[index_b1];

                        s_accum_r += lerpq(f128(col_r0), f128(col_r1), lerp_x) * smp_weight;
                        s_accum_g += lerpq(f128(col_g0), f128(col_g1), lerp_x) * smp_weight;
                        s_accum_b += lerpq(f128(col_b0), f128(col_b1), lerp_x) * smp_weight;
                    }
                    // Here we only interpolate along one axis, but not the other.
                    else if ((pos_x0 == pos_x1) && (pos_y0 != pos_y1))
                    {
                        u64 index_r0 = RGB_red(old_x, pos_x0, pos_y0);
                        u64 index_g0 = RGB_green(old_x, pos_x0, pos_y0);
                        u64 index_b0 = RGB_blue(old_x, pos_x0, pos_y0);

                        u64 index_r1 = RGB_red(old_x, pos_x1, pos_y1);
                        u64 index_g1 = RGB_green(old_x, pos_x1, pos_y1);
                        u64 index_b1 = RGB_blue(old_x, pos_x1, pos_y1);

                        f64 col_r0 = 0.0, col_g0 = 0.0, col_b0 = 0.0;
                        f64 col_r1 = 0.0, col_g1 = 0.0, col_b1 = 0.0;

                        #pragma omp atomic read
                        col_r0 = old_pixelarray[index_r0];
                        #pragma omp atomic read
                        col_g0 = old_pixelarray[index_g0];
                        #pragma omp atomic read
                        col_b0 = old_pixelarray[index_b0];

                        #pragma omp atomic read
                        col_r1 = old_pixelarray[index_r1];
                        #pragma omp atomic read
                        col_g1 = old_pixelarray[index_g1];
                        #pragma omp atomic read
                        col_b1 = old_pixelarray[index_b1];

                        s_accum_r += lerpq(f128(col_r0), f128(col_r1), lerp_y) * smp_weight;
                        s_accum_g += lerpq(f128(col_g0), f128(col_g1), lerp_y) * smp_weight;
                        s_accum_b += lerpq(f128(col_b0), f128(col_b1), lerp_y) * smp_weight;
                    }
                    // Here we have interpolation of some sort along both the x and y axis.
                    else
                    {
                        u64 index_r00 = RGB_red(old_x, pos_x0, pos_y0);
                        u64 index_g00 = RGB_green(old_x, pos_x0, pos_y0);
                        u64 index_b00 = RGB_blue(old_x, pos_x0, pos_y0);

                        u64 index_r10 = RGB_red(old_x, pos_x1, pos_y0);
                        u64 index_g10 = RGB_green(old_x, pos_x1, pos_y0);
                        u64 index_b10 = RGB_blue(old_x, pos_x1, pos_y0);

                        u64 index_r01 = RGB_red(old_x, pos_x0, pos_y1);
                        u64 index_g01 = RGB_green(old_x, pos_x0, pos_y1);
                        u64 index_b01 = RGB_blue(old_x, pos_x0, pos_y1);

                        u64 index_r11 = RGB_red(old_x, pos_x1, pos_y1);
                        u64 index_g11 = RGB_green(old_x, pos_x1, pos_y1);
                        u64 index_b11 = RGB_blue(old_x, pos_x1, pos_y1);

                        f64 col_r00 = 0.0, col_g00 = 0.0, col_b00 = 0.0;
                        f64 col_r10 = 0.0, col_g10 = 0.0, col_b10 = 0.0;
                        f64 col_r01 = 0.0, col_g01 = 0.0, col_b01 = 0.0;
                        f64 col_r11 = 0.0, col_g11 = 0.0, col_b11 = 0.0;

                        #pragma omp atomic read
                        col_r00 = old_pixelarray[index_r00];
                        #pragma omp atomic read
                        col_g00 = old_pixelarray[index_g00];
                        #pragma omp atomic read
                        col_b00 = old_pixelarray[index_b00];

                        #pragma omp atomic read
                        col_r10 = old_pixelarray[index_r10];
                        #pragma omp atomic read
                        col_g10 = old_pixelarray[index_g10];
                        #pragma omp atomic read
                        col_b10 = old_pixelarray[index_b10];

                        #pragma omp atomic read
                        col_r01 = old_pixelarray[index_r01];
                        #pragma omp atomic read
                        col_g01 = old_pixelarray[index_g01];
                        #pragma omp atomic read
                        col_b01 = old_pixelarray[index_b01];

                        #pragma omp atomic read
                        col_r11 = old_pixelarray[index_r11];
                        #pragma omp atomic read
                        col_g11 = old_pixelarray[index_g11];
                        #pragma omp atomic read
                        col_b11 = old_pixelarray[index_b11];

                        f128 mix_r0 = lerpq(f128(col_r00), f128(col_r10), lerp_x);
                        f128 mix_g0 = lerpq(f128(col_g00), f128(col_g10), lerp_x);
                        f128 mix_b0 = lerpq(f128(col_b00), f128(col_b10), lerp_x);

                        f128 mix_r1 = lerpq(f128(col_r01), f128(col_r11), lerp_x);
                        f128 mix_g1 = lerpq(f128(col_g01), f128(col_g11), lerp_x);
                        f128 mix_b1 = lerpq(f128(col_b01), f128(col_b11), lerp_x);

                        s_accum_r += lerpq(mix_r0, mix_r1, lerp_y) * smp_weight;
                        s_accum_g += lerpq(mix_g0, mix_g1, lerp_y) * smp_weight;
                        s_accum_b += lerpq(mix_b0, mix_b1, lerp_y) * smp_weight;
                    }

                    s_accum_weight += smp_weight;
                } // END OpenMP-enabled for loop

                // Calculate the final color values for the output pixel, from all of the samples that were added together and weighted.
                if (s_accum_weight > 0.0_q)
                {
                    s_accum_r /= s_accum_weight;
                    s_accum_g /= s_accum_weight;
                    s_accum_b /= s_accum_weight;
                }

                // Write the output pixel color data to the output buffer, 64-bits per color.
                {
                    u64 index_r = RGB_red(new_x, p1_x, p1_y);
                    u64 index_g = RGB_green(new_x, p1_x, p1_y);
                    u64 index_b = RGB_blue(new_x, p1_x, p1_y);

                    #pragma omp atomic write
                    new_pixelarray[index_r] = f64(s_accum_r);
                    #pragma omp atomic write
                    new_pixelarray[index_g] = f64(s_accum_g);
                    #pragma omp atomic write
                    new_pixelarray[index_b] = f64(s_accum_b);
                }

            } // END OpenMP-enabled for (pixel_index = pixel_start - pixel_end)

        } // END OpenMP-enabled for (int chunk_index = 0; chunk_index <= progress_steps; chunk_index++)

        // Write output drawable
        {
            const GeglRectangle* rect = gegl_rectangle_new((gint)0, (gint)0, (guint)new_x, (guint)new_y);
            const Babl* format = babl_format_with_space("RGB double", NULL);
            double* pxl = new double[new_total * 3u];
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
