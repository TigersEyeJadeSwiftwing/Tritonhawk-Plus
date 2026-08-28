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

#include "components/thp_types.hpp"
#include "components/thp_resize.hpp"
#include "components/thp_log.hpp"
#include "components/thp_parameters.hpp"
#include "components/thp_operation.hpp"
#include "components/thp_gui-combowidget-size.hpp"
// #include "tritonhawk-plus.hpp"

#include <omp.h>

#include <string>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;
using namespace TritonhawkPlus;
using namespace quadmath;

extern ThpLog* Log;
extern ThpParams* Params;

namespace TritonhawkPlus
{
    TARGET_CLONES bool Thp_Operation__Complete_Image__Magic_Shrink()
    {
        if (!Params)
            return false;

        if (!Params->image)
        {
            Params->operation_result_string = "Error: No image detected.";
            return false;
        }

        Params->EngagePluginPriority();

        Params->gui_enabled = false;

        gimp_image_undo_group_start(Params->image);
        gimp_context_push();

        gimp_context_set_interpolation (GIMP_INTERPOLATION_NONE);

        GimpLayer** image_layers = gimp_image_get_layers((GimpImage*)Params->image);
        s32 number_of_layers = 0;

        if (!image_layers)
        {
            Params->operation_result_string = "Error: No layers detected in image.";
            gimp_context_pop();
            gimp_displays_flush();
            gimp_image_undo_group_end(Params->image);
            return false;
        }

        for (s32 i = 0; i < 5000; i++)
        {
            if (image_layers[i])
                number_of_layers++;
            else
                break;
        }

        if (number_of_layers < 1)
        {
            Params->operation_result_string = "Error: Number of layers detected is zero.";
            gimp_context_pop();
            gimp_displays_flush();
            gimp_image_undo_group_end(Params->image);
            return false;
        }

        gimp_progress_init( (gchar*)("Shrinking...") );

        for (s32 layer_index = 0; layer_index < number_of_layers; layer_index++)
        {
            if (number_of_layers > 1)
            {
                gimp_progress_set_text( g_strdup_printf(_("Shrinking, layer %i / %i..."), layer_index, number_of_layers ) );
                gimp_progress_update( gdouble(layer_index / number_of_layers) );
            }

            // Set pointers for the current image's "layer" and the layer's "drawable", as GIMP calls them.
            GimpLayer* layer = image_layers[layer_index];
            GimpDrawable* layer_drawable = (GimpDrawable*)layer;

            // If the current layer is a text layer, convert it into a regular layer of pixels before going any further.
            if ( gimp_item_is_text_layer((GimpItem*)layer) == TRUE )
                gimp_rasterizable_rasterize((GimpRasterizable*)layer);

            // Check if the current layer has any x or y dimension offset.  If it has any offset of either or both
            // dimensions, set both to zero.
            gimp_layer_set_offsets(
                layer,
                (gint)0, (gint)0
            );

            // Get the x and y (width and height) dimensions of the current layer, and calculate the new width and
            // height of the layer to resize it to, based upon the smaller of the two dimensions.
            gint layer_size_x = (gint) gimp_drawable_get_width(layer_drawable);
            gint layer_size_y = (gint) gimp_drawable_get_height(layer_drawable);
            Params->input_size_x = (u64)layer_size_x;
            Params->input_size_y = (u64)layer_size_y;
            gint layer_smallest_dimension = min(layer_size_x, layer_size_y);
            layer_smallest_dimension /= 8;
            layer_smallest_dimension = max(1, layer_smallest_dimension);
            Params->output_size_x = (u64)layer_smallest_dimension;
            Params->output_size_y = (u64)layer_smallest_dimension;

            // Update the paramaters with the information on what we're doing
            Params->CopySizesToLayerSizes();
            Params->CalcAll();

            if ((Log) && (layer_index == 0))
                Log->SetTimerStart();

            // Actually resize the layer, accounting for whether the layer has an alpha channel or not.
            if (gimp_drawable_has_alpha(layer_drawable) == TRUE)
                Thp_resize_layer_RGBA(Params, layer);
            else
                Thp_resize_layer_RGB(Params, layer);
        }

        // We don't need the list of image layers anymore, so we free up memory by getting rid of it.
        g_free(image_layers);

        // Resize the image so that the image's size (width and height dimensions) can fit all of the layers inside of it.
        gimp_image_resize_to_layers(Params->image);

        gimp_context_pop();
        gimp_displays_flush();
        gimp_image_undo_group_end(Params->image);

        return true;
    }
}
