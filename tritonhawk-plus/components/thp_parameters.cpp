#include "components/thp_types.h"
#include "components/thp_parameters.h"

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
    void ThpParams::CalcAll()
    {
        CalcThreads();
        CalcNumberOfChunks();
        CalcSampleGrid();
        CalcInfoString();
    }
    void ThpParams::CalcInfoString()
    {
        info_string = (string) g_strdup_printf (
            _(
                "----------" "----------" "----------" "----------" "----------" "----------" "----------" "----------" "----------" "----------" "----------" "----------" "\n"
                "----- Tritonhawk Plus GIMP Plug-in: %s" "\n"
                "-       High Quality, Multi-threaded, using OpenMP technology.  Uses 128-bit floating-point math for processing." "\n"
                "-" "\n"
                "-     Enabled threads for multi-threaded: %i" "\n"
                "-     Number of drawables to process in image: %i" "\n"
                "-       Old image_size (all layers): %i x %i pixels, %i total pixels" "\n"
                "-       New image_size (all layers): %i x %i pixels, %i total pixels" "\n"
                "-     Seamless tiling X: %s, Seamless tiling Y: %s" "\n"
                "-     Sample size adjustment level: %%%3.5lf" "\n"
                "-       Sample grid size: %i x %i, total samples: %i" "\n"
                "-       Sample grid scale: %%%3.2lf x %%%3.2lf" "\n"
                "-     Default Chunk size: %i kilosamples (%i samples)" "\n"
                "-       Actual Chunk size: %i pixels / %i samples" "\n"
                "-       Default number of chunks to process: %i (%i total samples)" "\n"
            ),
            process_name.c_str(),
            number_threads,
            draw_count,
            input_size_x, input_size_y, input_size_xy,
            output_size_x, output_size_y, output_size_xy,
            seamless_x ? "Enabled" : "Inactive", seamless_y ? "Enabled" : "Inactive",
            (f64)sample_count_adjustment * 100.0,
            sample_count_x, sample_count_y, sample_count_xy,
            sample_grid_width_percent, sample_grid_height_percent,
            chunk_size_default / 1024, chunk_size_default,
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
        f128 grid_scale_x = f128(sample_grid_width_percent) * 0.01Q;
        f128 grid_scale_y = f128(sample_grid_height_percent) * 0.01Q;

        if (input_size_x > output_size_x)
        {
            f128 scale_factor = ((grid_scale_x * image_ratio_x) - 1.00001Q) / 2.0Q;
            int additional_samples = int(scale_factor) * 2;

            sample_count_x = 3 + additional_samples;
        }
        else
        {
            if (grid_scale_x > 1.00001Q)
            {
                f128 scale_factor = (grid_scale_x) * 0.5Q;
                int additional_samples = int(scale_factor) * 2;

                sample_count_x = 3 + additional_samples;
            }
            else
                sample_count_x = 1;
        }

        if (input_size_y > output_size_y)
        {
            f128 scale_factor = ((grid_scale_y * image_ratio_y) - 1.00001Q) / 2.0Q;
            int additional_samples = int(scale_factor) * 2;

            sample_count_y = 3 + additional_samples;
        }
        else
        {
            if (grid_scale_y > 1.00001Q)
            {
                f128 scale_factor = (grid_scale_y) * 0.5Q;
                int additional_samples = int(scale_factor) * 2;

                sample_count_y = 3 + additional_samples;
            }
            else
                sample_count_y = 1;
        }

        if (sample_count_adjustment > 1.0Q)
        {
            sample_count_x = to_intq(f128(sample_count_x) * sample_count_adjustment);
            sample_count_y = to_intq(f128(sample_count_y) * sample_count_adjustment);
        }

        sample_count_xy = sample_count_x * sample_count_y;

        const f128 r_x = fmaxq(image_ratio_x, 0.50Q);
        const f128 r_y = fmaxq(image_ratio_y, 0.50Q);
        sample_grid_scale_x = grid_scale_x * r_x;
        sample_grid_scale_y = grid_scale_y * r_y;

        sample_grid_scale_x = 0.0Q;
        sample_grid_scale_y = 0.0Q;
        sample_grid_offset_x = 0.Q;
        sample_grid_offset_y = 0.Q;
    }

    void ThpParams::CalcNumberOfChunks()
    {
        sample_count_xy = max(sample_count_xy, 1);
        chunk_size_default = max(chunk_size_default, 1024);
        chunk_size_samples = chunk_size_default;
        input_size_x = min(max(input_size_x, 1), max_image_dimension);
        input_size_y = min(max(input_size_y, 1), max_image_dimension);
        input_size_xy = input_size_x * input_size_y;
        output_size_x = min(max(output_size_x, 1), max_image_dimension);
        output_size_y = min(max(output_size_y, 1), max_image_dimension);
        output_size_xy = output_size_x * output_size_y;
        image_ratio_x = f128(input_size_x) / f128(output_size_x);
        image_ratio_y = f128(input_size_y) / f128(output_size_y);
        image_ratio_xy = f128(input_size_xy) / f128(output_size_xy);

        total_samples = output_size_xy * sample_count_xy;
        chunk_size_pixels = max(chunk_size_samples / sample_count_xy, 1) + 1;
        number_chunks = (output_size_xy / chunk_size_pixels) + 1;
        if (number_chunks < number_threads)
        {
            f128 chunk_size_ratio = f128(number_threads) / f128(number_chunks);
            f128 chunk_size_pixels_f = f128(chunk_size_pixels) / chunk_size_ratio;
            chunk_size_pixels = int(chunk_size_pixels_f) + 1;
            number_chunks = (output_size_xy / chunk_size_pixels) + 1;
        }
        if ((number_chunks * chunk_size_pixels) < output_size_xy)
        {
            int pixels_diff = output_size_xy - (number_chunks * chunk_size_pixels);
            int chunks_diff = pixels_diff / chunk_size_pixels;
            number_chunks += chunks_diff + 1;
        }

        progress_increment = (progress_end - progress_start) / f64(number_chunks);
    }
};
