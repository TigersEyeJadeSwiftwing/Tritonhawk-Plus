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
    TARGET_CLONES bool Thp_Operation__Resize()
    {
        if (!Params)
            return false;

        if ((!Params->image) || (!Params->drawables))
        {
            Params->operation_result_string = "Error: No image or layers to resize.";
            return false;
        }

        if (Params->run_mode_interactive == false)
        {
            Params->operation_result_string = "Error: This plugin doesn't (yet) run without the GUI.";
            return false;
        }

        GimpLayer** image_layers = 0;
        s32 number_of_layers = 0;
        s32 number_of_layers_to_process = 0;
        bool image_layers_gfree_needed = false;

        if (Params->layers_to_process == LAYERS_TO_PROCESS__ACTIVE_SELECTION)
            image_layers = (GimpLayer**)Params->drawables;
        else if (Params->layers_to_process == LAYERS_TO_PROCESS__ACTIVE_LAYER)
            image_layers = (GimpLayer**)Params->drawables;
        else if (Params->layers_to_process == LAYERS_TO_PROCESS__ALL_VISIBLE_LAYERS)
        {
            image_layers = gimp_image_get_layers((GimpImage*)Params->image);
            image_layers_gfree_needed = true;
        }
        else if (Params->layers_to_process == LAYERS_TO_PROCESS__ALL_LAYERS)
        {
            image_layers = gimp_image_get_layers((GimpImage*)Params->image);
            image_layers_gfree_needed = true;
        }
        else
            image_layers = (GimpLayer**)Params->drawables;

        GimpLayer* layer = 0;

        for (int i = 0; i < 5000; i++)
        {
            if (image_layers[i])
            {
                if (Params->layers_to_process == LAYERS_TO_PROCESS__ALL_LAYERS)
                {
                    number_of_layers++;
                    number_of_layers_to_process++;
                    layer = image_layers[i];
                }
                else if (Params->layers_to_process == LAYERS_TO_PROCESS__ALL_VISIBLE_LAYERS)
                {
                    number_of_layers++;

                    if (gimp_item_get_visible( (GimpItem*)image_layers[i] ) == TRUE)
                    {
                        number_of_layers_to_process++;
                        layer = image_layers[i];
                    }
                }
                else
                {
                    number_of_layers++;
                    number_of_layers_to_process++;
                    layer = image_layers[i];
                }
            }
            else
                break;
        }

        if (number_of_layers_to_process < 1)
        {
            Params->operation_result_string = "Error: No visible layers to resize.";
            return false;
        }
        if (number_of_layers_to_process > 1)
        {
            layer = image_layers[0];
        }

        Params->draw_count = number_of_layers;

        bool image_has_multiple_layers = false;
        if (number_of_layers > 1)
            image_has_multiple_layers = true;
        else
        {
            GimpLayer** layer_list = gimp_image_get_layers(Params->image);
            if (layer_list[1])
                image_has_multiple_layers = true;

            g_free(layer_list);
        }

        if (Params->draw_count == 0)
        {
            Params->operation_result_string = "Error: No layers to resize.";
            return false;
        }
        else if (Params->draw_count == 1)
            Params->multiple_image_layers = false;
        else
            Params->multiple_image_layers = true;

        GimpImage* image = Params->image;
        // GimpLayer* layer = image_layers[0];
        GimpDrawable* drawable = (GimpDrawable*)layer;

        if (Params->multiple_image_layers == false)
        {
            number_of_layers = 1;
            Params->draw_count = number_of_layers;

            if (Params->drawables)
            {
                if (Params->drawables[0])
                {
                    layer = (GimpLayer*)Params->drawables[0];
                    drawable = Params->drawables[0];

                    /*
                    if ( gimp_layer_is_floating_sel((GimpLayer*)Params->drawables[0]) == (gboolean)TRUE )
                    {
                        layer = (GimpLayer*)Params->drawables[0];
                        drawable = Params->drawables[0];
                    }
                    */
                }
                else
                {
                    Params->operation_result_string = "Error: No layers to resize.";
                    return false;
                }
            }
            else
            {
                Params->operation_result_string = "Error: No layers to resize.";
                return false;
            }
        }

        f64 progress_size = 1.0 / f64(number_of_layers);

        gint image_size_x = (gint) 0;
        gint image_size_y = (gint) 0;
        gint layer_size_x = (gint) 0;
        gint layer_size_y = (gint) 0;
        gint offset_old_x = (gint) 0;
        gint offset_old_y = (gint) 0;
        gint offset_x = (gint) 0;
        gint offset_y = (gint) 0;

        image_size_x = (gint) gimp_image_get_width( image );
        image_size_y = (gint) gimp_image_get_height( image );
        Params->image_input_size_x = image_size_x;
        Params->image_input_size_y = image_size_y;
        Params->image_input_size_xy = image_size_x * image_size_y;

        layer_size_x = (gint) gimp_drawable_get_width( drawable );
        layer_size_y = (gint) gimp_drawable_get_height( drawable );

        gimp_drawable_get_offsets(
            drawable,
            &offset_old_x, &offset_old_y
        );

        // layer_size_x += offset_old_x;
        // layer_size_y += offset_old_y;

        Params->layer_input_size_x = layer_size_x;
        Params->layer_input_size_y = layer_size_y;
        Params->layer_input_size_xy = layer_size_x * layer_size_y;

        /*
        Params->input_size_x = (u64)layer_size_x;
        Params->input_size_y = (u64)layer_size_y;
        Params->output_size_x = Params->input_size_x;
        Params->output_size_y = Params->input_size_y;
        */
        Params->chunk_size_default = (u64)5000uLL;
        Params->chunk_size_kilo = (u64)10uLL;

        Params->image_output_size_x = Params->image_input_size_x;
        Params->image_output_size_y = Params->image_input_size_y;
        Params->image_output_size_xy = Params->image_input_size_xy;
        Params->layer_output_size_x = Params->layer_input_size_x;
        Params->layer_output_size_y = Params->layer_input_size_y;
        Params->layer_output_size_xy = Params->layer_input_size_xy;

        Params->SetOutputSizeDefaults();
        Params->CalcAll();

        Log = new ThpLog();

        GtkWidget*              Program_Dialog;
        GtkWidget*              Gui_Log_Box_0;
        GtkWidget*              Gui_Log_Text_0;
        GtkWidget*              Gui_Log_Text_1;
        ComboSizeWidget*        Combo_Size_Widget;
        GdkGeometry             geometry;

        gimp_ui_init (PLUG_IN_BINARY);
        Program_Dialog = gimp_dialog_new (
            Params->process_name.c_str(),
            PLUG_IN_ROLE,
            NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            NULL,
            NULL,
            NULL_TERMINATE
        );

        geometry.min_aspect = 0.125;
        geometry.max_aspect = 8.0;
        geometry.base_width = (gint)660;
        geometry.width_inc = (gint)660;
        geometry.min_width = (gint)660;
        geometry.max_width = (gint)4096;
        geometry.base_height = (gint)750;
        geometry.height_inc = (gint)750;
        geometry.min_height = (gint)500;
        geometry.max_height = (gint)4096;
        gtk_window_set_geometry_hints (GTK_WINDOW (Program_Dialog), NULL, &geometry, GDK_HINT_ASPECT);

        Log->SetGuiDialog(Program_Dialog);

        Gui_Log_Box_0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Log_Box_0), 5);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Log_Box_0), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Log_Box_0), GTK_ALIGN_START);
        gtk_widget_set_size_request(Gui_Log_Box_0, 800, 240);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG (Program_Dialog))), Gui_Log_Box_0);
        gtk_widget_show(Gui_Log_Box_0);

        Gui_Log_Text_0 = gtk_label_new (NULL);
        gtk_widget_set_size_request (Gui_Log_Text_0, 400, 240);
        gtk_widget_set_halign ( GTK_WIDGET(Gui_Log_Text_0), GTK_ALIGN_START );
        gtk_widget_set_valign ( GTK_WIDGET(Gui_Log_Text_0), GTK_ALIGN_START );
        gtk_box_pack_start(GTK_BOX(Gui_Log_Box_0), (GtkWidget*)Gui_Log_Text_0, FALSE, FALSE, 0);
        gtk_widget_show (Gui_Log_Text_0);
        Log->SetTextLabel(Gui_Log_Text_0);

        Gui_Log_Text_1 = gtk_label_new (NULL);
        gtk_widget_set_size_request (Gui_Log_Text_1, 400, 240);
        gtk_widget_set_halign ( GTK_WIDGET(Gui_Log_Text_1), GTK_ALIGN_START );
        gtk_widget_set_valign ( GTK_WIDGET(Gui_Log_Text_1), GTK_ALIGN_START );
        gtk_box_pack_start(GTK_BOX(Gui_Log_Box_0), (GtkWidget*)Gui_Log_Text_1, FALSE, FALSE, 0);
        gtk_widget_show (Gui_Log_Text_1);
        Log->SetTextLabel(Gui_Log_Text_1, 1);

        gtk_widget_show (Program_Dialog);

        Params->gui_enabled = true;

        Combo_Size_Widget = new ComboSizeWidget(Program_Dialog, Params, Log);
        Combo_Size_Widget->SetOriginalSize((gint)Params->input_size_x, (gint)Params->input_size_y);

        gtk_window_set_default_size(GTK_WINDOW (Program_Dialog), -1, -1);

        Log->Log(false, Params->info_string);

        while (Combo_Size_Widget->GetChoicesDoneResult() == 0)
        {
            while (gtk_events_pending())
                gtk_main_iteration();
        };

        if (Combo_Size_Widget->GetChoicesDoneResult() == -1)
        {
            // delete Log;
            // delete Params;
            if (Combo_Size_Widget) delete Combo_Size_Widget;

            return true;

            // return gimp_procedure_new_return_values (Params->procedure, GIMP_PDB_CANCEL, NULL);
        }

        gimp_context_push ();

        Params->EngagePluginPriority();
        gtk_widget_set_sensitive((GtkWidget*)Program_Dialog, (gboolean)FALSE);

        gimp_context_set_interpolation (GIMP_INTERPOLATION_NONE);

        // Log->SetTimerStart();

        if (number_of_layers == 1)
        {
            // If the end-user uses the "undo" command for the image, everything from the "gimp_image_undo_group_start" function to
            // the "gimp_image_undo_group_end" function will be a single undo operation, and also show up in GIMP's undo history as
            // a single entry in a list of commands and operations on the image.
            gimp_image_undo_group_start(image);

            // If the current layer is a text layer, convert it into a regular layer of pixels before going any further.
            if ( gimp_item_is_text_layer((GimpItem*)layer) == TRUE )
            {
                gimp_rasterizable_rasterize((GimpRasterizable*)layer);
            }

            // Check if the current layer has any x or y dimension offset.  If it has any offset of either or both
            // dimensions, set both to zero, for now.
            if ( (offset_old_x != (gint)0) || (offset_old_y != (gint)0) )
            {
                gimp_layer_set_offsets(
                    layer,
                    (gint)0, (gint)0
                );

                offset_x = (gint)(f128( (f128)offset_old_x * (f128)Params->layer_output_size_x / (f128)Params->layer_input_size_x ));
                offset_y = (gint)(f128( (f128)offset_old_y * (f128)Params->layer_output_size_y / (f128)Params->layer_input_size_y ));
            }

            // Update the paramaters with the information on what we're doing with this later of the image.
            Params->draw_index = 0;
            Params->CopySizesToLayerSizes();
            Params->CalcAll();

            Combo_Size_Widget->SyncDataFromParameters();

            Log->SetTimerStart();

            // Actually resize the layer, accounting for whether the layer has an alpha channel or not.
            if (gimp_drawable_has_alpha(drawable) == TRUE)
                Thp_resize_layer_RGBA(Params, layer);
            else
                Thp_resize_layer_RGB(Params, layer);

            if ( (offset_old_x != (gint)0) || (offset_old_y != (gint)0) )
            {
                gimp_layer_set_offsets(
                    layer,
                    (gint)offset_x, (gint)offset_y
                );
            }

            if (image_has_multiple_layers == false)
                gimp_image_resize(
                    image,
                    (gint)Params->output_size_x + (gint)offset_x, (gint)Params->output_size_y + (gint)offset_y,
                    (gint)0, (gint)0
                );

            // We don't need the list of image layers anymore, so we free up memory by getting rid of it.
            if (image_layers_gfree_needed == true)
                g_free(image_layers);
            // If the end-user chooses to "undo" all the work we did, it does it to all the stuff before this line as one operation,
            // as explained above (earlier in the source code).
            gimp_image_undo_group_end(image);

            gimp_context_pop();
            gimp_displays_flush();
            gtk_widget_set_sensitive((GtkWidget*)Program_Dialog, (gboolean)TRUE);

            if (Combo_Size_Widget)
                delete Combo_Size_Widget;

            Params->DisengagePluginPriority();

            return true;
        }
        else if (Params->run_mode == RUN_MODE_RESIZE__ALL_LAYERS_SAME_RATIO_V2)
        {
            // If the end-user uses the "undo" command for the image, everything from the "gimp_image_undo_group_start" function to
            // the "gimp_image_undo_group_end" function will be a single undo operation, and also show up in GIMP's undo history as
            // a single entry in a list of commands and operations on the image.
            gimp_image_undo_group_start(image);

            /*
            Params->input_size_x = Params->image_input_size_x;
            Params->input_size_y = Params->image_input_size_y;
            Params->output_size_x = Params->image_output_size_x;
            Params->output_size_y = Params->image_output_size_y;
            */

            // If the image isn't being resized from the original in either x or y dimensions, or both, this detects it and
            // takes it into account, to simplify the image processing.
            f128 scale_x_f = 1.0q;
            f128 scale_y_f = 1.0q;
            bool resize_equal_x = true;
            bool resize_equal_y = true;
            if (Params->output_size_x != Params->input_size_x)
            {
                resize_equal_x = false;
                scale_x_f = f128(Params->output_size_x) / f128(Params->input_size_x);
            }
            if (Params->output_size_y != Params->input_size_y)
            {
                resize_equal_y = false;
                scale_y_f = f128(Params->output_size_y) / f128(Params->input_size_y);
            }

            // Iterate through (Go one-by-one, in-order, through) all of the layers of the image, and resize them.
            for (s32 layer_index = 0; layer_index < number_of_layers; layer_index++)
            {
                // Set pointers for the current image's "layer" and the layer's "drawable", as GIMP calls them.
                layer = image_layers[layer_index];
                GimpDrawable* layer_drawable = (GimpDrawable*)layer;

                // If the current layer is a text layer, convert it into a regular layer of pixels before going any further.
                if ( gimp_item_is_text_layer((GimpItem*)layer) == TRUE )
                {
                    gimp_rasterizable_rasterize((GimpRasterizable*)layer);
                }

                // If the GUI is enabled, update the progress percentages completed, for both the current layer, and also
                // all of the layers altogether.  Also, update the Parameter variable for which layer is being processed,
                // for the GUI display.
                if (Params->gui_enabled == true)
                {
                    f64 progress_start = f64(layer_index) / f64(number_of_layers);
                    Params->progress_start = (f64)progress_start;
                    Params->progress_end = f64(progress_start + progress_size);

                    Params->draw_index = layer_index;
                }

                // Check if the current layer has any x or y dimension offset.  If it has any offset of either or both
                // dimensions, set both to zero, for now.
                offset_old_x = (gint) 0;
                offset_old_y = (gint) 0;
                offset_x = (gint) 0;
                offset_y = (gint) 0;
                gimp_drawable_get_offsets(
                    layer_drawable,
                    &offset_old_x, &offset_old_y
                );
                if ( (offset_old_x != (gint)0) || (offset_old_y != (gint)0) )
                {
                    gimp_layer_set_offsets(
                        layer,
                        (gint)0, (gint)0
                    );
                }

                // Get the x and y (width and height) dimensions of the current layer, and calculate the new width and
                // height of the layer to resize it to, based upon the same x and y scale as what was chosen by the user.
                gint layer_size_x = (gint) gimp_drawable_get_width(layer_drawable);
                gint layer_size_y = (gint) gimp_drawable_get_height(layer_drawable);
                if (resize_equal_x == true) {
                    Params->input_size_x = (u64)layer_size_x;
                    Params->output_size_x = (u64)layer_size_x;
                } else {
                    f128 new_layer_size_x_f = f128(layer_size_x) * scale_x_f;
                    Params->input_size_x = (u64)layer_size_x;
                    Params->output_size_x = (u64)max( (u64)roundq(new_layer_size_x_f), 1uLL );
                }
                if (resize_equal_y == true) {
                    Params->input_size_y = (u64)layer_size_y;
                    Params->output_size_y = (u64)layer_size_y;
                } else {
                    f128 new_layer_size_y_f = f128(layer_size_y) * scale_y_f;
                    Params->input_size_y = (u64)layer_size_y;
                    Params->output_size_y = (u64)max( (u64)roundq(new_layer_size_y_f), 1uLL );
                }

                // Update the paramaters with the information on what we're doing with this later of the image.
                Params->CopySizesToLayerSizes();
                Params->CalcAll();

                // If the GUI is enabled, update the GUI's display on-screen.
                if (Params->gui_enabled == true)
                {
                    Combo_Size_Widget->SyncDataFromParameters();
                }

                if (layer_index == 0)
                    Log->SetTimerStart();

                // Actually resize the layer, accounting for whether the layer has an alpha channel or not.
                if (gimp_drawable_has_alpha(layer_drawable) == TRUE)
                    Thp_resize_layer_RGBA(Params, layer);
                else
                    Thp_resize_layer_RGB(Params, layer);

                // If the layer had any left-right or width offset to begin with, calculate the new "x" dimension offset.
                if (resize_equal_x == true)
                    offset_x = offset_old_x;
                else
                    offset_x = (gint) (f128( f128(offset_old_x) * scale_x_f ));

                // If the layer had any up-down or height offset to begin with, calculate the new "y" dimension offset.
                if (resize_equal_y == true)
                    offset_y = offset_old_y;
                else
                    offset_y = (gint) (f128( f128(offset_old_y) * scale_y_f ));

                // If the layer had any x or y dimension offsets, or both, to begin with, offset the layer to the new offset values.
                if ( (offset_old_x != (gint)0) || (offset_old_y != (gint)0) )
                {
                    gimp_layer_set_offsets(
                        layer,
                        offset_x, offset_y
                    );
                }

            } // END of looping through all layers of the image.

            // We don't need the list of image layers anymore, so we free up memory by getting rid of it.
            if (image_layers_gfree_needed == true)
                g_free(image_layers);
            // Resize the image so that the image's size (width and height dimensions) can fit all of the layers inside of it.
            gimp_image_resize_to_layers(image);
            // If the end-user chooses to "undo" all the work we did, it does it to all the stuff before this line as one operation,
            // as explained above (earlier in the source code).
            gimp_image_undo_group_end(image);

            gimp_context_pop();
            gimp_displays_flush();
            gtk_widget_set_sensitive((GtkWidget*)Program_Dialog, (gboolean)TRUE);

            if (Combo_Size_Widget)
                delete Combo_Size_Widget;

            Params->DisengagePluginPriority();

            return true;
        }
        else if (Params->run_mode == RUN_MODE_RESIZE__ALL_LAYERS_SAME_DIMENSIONS_V2)
        {
            // If the end-user uses the "undo" command for the image, everything from the "gimp_image_undo_group_start" function to
            // the "gimp_image_undo_group_end" function will be a single undo operation, and also show up in GIMP's undo history as
            // a single entry in a list of commands and operations on the image.
            gimp_image_undo_group_start(image);

            /*
            Params->input_size_x = Params->image_input_size_x;
            Params->input_size_y = Params->image_input_size_y;
            Params->output_size_x = Params->image_output_size_x;
            Params->output_size_y = Params->image_output_size_y;
            */

            // Iterate through (Go one-by-one, in-order, through) all of the layers of the image, and resize them.
            for (s32 layer_index = 0; layer_index < number_of_layers; layer_index++)
            {
                // Set pointers for the current image's "layer" and the layer's "drawable", as GIMP calls them.
                layer = image_layers[layer_index];
                GimpDrawable* layer_drawable = (GimpDrawable*)layer;

                // If the current layer is a text layer, convert it into a regular layer of pixels before going any further.
                if ( gimp_item_is_text_layer((GimpItem*)layer) == TRUE )
                {
                    gimp_rasterizable_rasterize((GimpRasterizable*)layer);
                }

                // If the GUI is enabled, update the progress percentages completed, for both the current layer, and also
                // all of the layers altogether.  Also, update the Parameter variable for which layer is being processed,
                // for the GUI display.
                if (Params->gui_enabled == true)
                {
                    f64 progress_start = f64(layer_index) / f64(number_of_layers);
                    Params->progress_start = (f64)progress_start;
                    Params->progress_end = f64(progress_start + progress_size);

                    Params->draw_index = layer_index;
                }

                if (Params->whole_image_mode == false)
                {
                    gimp_drawable_get_offsets(
                        drawable,
                        &offset_old_x, &offset_old_y
                    );
                }

                // Set the current layer to have an offset of zero pixels, in both x and y dimensions.
                gimp_layer_set_offsets(
                    layer,
                    (gint)0, (gint)0
                );

                // Get the x and y (width and height) dimensions of the current layer, and update the parameters to reflect this.
                Params->input_size_x = (u64)gimp_drawable_get_width(layer_drawable);
                Params->input_size_y = (u64)gimp_drawable_get_height(layer_drawable);

                // Update the paramaters with the information on what we're doing with this later of the image.
                Params->CopySizesToLayerSizes();
                Params->CalcAll();

                // If the GUI is enabled, update the GUI's display on-screen.
                if (Params->gui_enabled == true)
                    Combo_Size_Widget->SyncDataFromParameters();

                if (layer_index == 0)
                    Log->SetTimerStart();

                // Actually resize the layer, accounting for whether the layer has an alpha channel or not.
                if (gimp_drawable_has_alpha(layer_drawable) == TRUE)
                    Thp_resize_layer_RGBA(Params, layer);
                else
                    Thp_resize_layer_RGB(Params, layer);

                if (Params->whole_image_mode == false)
                {
                    // If there is any offset to apply to the layer, now that we've resized it, apply it.
                    if ((offset_old_x != 0) || (offset_old_y != 0))
                        gimp_layer_set_offsets(
                            layer,
                            (gint)offset_old_x, (gint)offset_old_y
                        );
                }

            } // END of looping through all layers of the image.

            // We don't need the list of image layers anymore, so we free up memory by getting rid of it.
            if (image_layers_gfree_needed == true)
                g_free(image_layers);
            // Resize the image so that the image's size (width and height dimensions) can fit all of the layers inside of it.
            gimp_image_resize_to_layers(image);
            // If the end-user chooses to "undo" all the work we did, it does it to all the stuff before this line as one operation,
            // as explained above (earlier in the source code).
            gimp_image_undo_group_end(image);

            gimp_context_pop();
            gimp_displays_flush();
            gtk_widget_set_sensitive((GtkWidget*)Program_Dialog, (gboolean)TRUE);

            if (Combo_Size_Widget)
                delete Combo_Size_Widget;

            Params->DisengagePluginPriority();

            return true;
        }
        else if (Params->run_mode == RUN_MODE_RESIZE__KEEP_ASPECT_SAME_VERTICAL_V2)
        {
            // If the end-user uses the "undo" command for the image, everything from the "gimp_image_undo_group_start" function to
            // the "gimp_image_undo_group_end" function will be a single undo operation, and also show up in GIMP's undo history as
            // a single entry in a list of commands and operations on the image.
            gimp_image_undo_group_start(image);

            /*
            Params->input_size_x = Params->image_input_size_x;
            Params->input_size_y = Params->image_input_size_y;
            Params->output_size_x = Params->image_output_size_x;
            Params->output_size_y = Params->image_output_size_y;
            */

            s32 output_size_param_x = (s32)Params->output_size_x;
            s32 output_size_param_y = (s32)Params->output_size_y;

            // Iterate through (Go one-by-one, in-order, through) all of the layers of the image, and resize them.
            for (s32 layer_index = 0; layer_index < number_of_layers; layer_index++)
            {
                // Set pointers for the current image's "layer" and the layer's "drawable", as GIMP calls them.
                layer = image_layers[layer_index];
                GimpDrawable* layer_drawable = (GimpDrawable*)layer;

                // If the current layer is a text layer, convert it into a regular layer of pixels before going any further.
                if ( gimp_item_is_text_layer((GimpItem*)layer) == TRUE )
                {
                    gimp_rasterizable_rasterize((GimpRasterizable*)layer);
                }

                if (Params->whole_image_mode == false)
                {
                    gimp_drawable_get_offsets(
                        drawable,
                        &offset_old_x, &offset_old_y
                    );
                }

                // Set the current layer to have an offset of zero pixels, in both x and y dimensions.
                gimp_layer_set_offsets(
                    layer,
                    (gint)0, (gint)0
                );

                s32 out_x, out_y, layer_x, layer_y;
                f128 layer_aspect, out_x_f;

                if (Params->whole_image_mode == true)
                {
                    // Grab the values of the new height and width, or x and y dimensions, of what to resize layers to, and keep
                    // the "y" dimension, which is the height to resize all layers of the image to, the same, while maintaining
                    // the aspect ratio of all of the layers of the image, which means that the width, or the "x" dimension, can vary,
                    // and has to be recalculated for each layer independently.  The initial width or "x" dimension to resize layers to,
                    // which was chosen in the GUI by the end-user, is actually ignored and is a value that isn't used.
                    out_x = (s32)Params->image_output_size_x;
                    out_y = (s32)Params->image_output_size_y;
                    layer_x = (s32)gimp_drawable_get_width(layer_drawable);
                    layer_y = (s32)gimp_drawable_get_height(layer_drawable);
                    layer_aspect = (f128)layer_x / (f128)layer_y;
                    out_x_f = f128(out_y) * layer_aspect;
                    out_x = s32(out_x_f);
                }
                else
                {
                    out_x = output_size_param_x;
                    out_y = output_size_param_y;
                    layer_x = (s32)gimp_drawable_get_width(layer_drawable);
                    layer_y = (s32)gimp_drawable_get_height(layer_drawable);
                    layer_aspect = (f128)layer_x / (f128)layer_y;
                    out_x_f = f128(out_y) * layer_aspect;
                    out_x = s32(out_x_f);
                }

                // Update the parameters for the starting x and y dimensions, and also for the x and y dimensions to resize to,
                // for the current layer, now that we've figured out what they are.
                Params->input_size_x = (u64)layer_x;
                Params->input_size_y = (u64)layer_y;
                Params->output_size_x = (u64)out_x;
                Params->output_size_y = (u64)out_y;

                // If the GUI is enabled, update the progress percentages completed, for both the current layer, and also
                // all of the layers altogether.  Also, update the Parameter variable for which layer is being processed,
                // for the GUI display.
                if (Params->gui_enabled == true)
                {
                    f64 progress_start = f64(layer_index) / f64(number_of_layers);
                    Params->progress_start = (f64)progress_start;
                    Params->progress_end = f64(progress_start + progress_size);

                    Params->draw_index = layer_index;
                }

                // Update the paramaters with the information on what we're doing with this later of the image.
                Params->CopySizesToLayerSizes();
                Params->CalcAll();

                // If the GUI is enabled, update the GUI's display on-screen.
                if (Params->gui_enabled == true)
                    Combo_Size_Widget->SyncDataFromParameters();

                // Calculate the new offset of the x dimension, if any.  Cutting it in half will produce an end-result where
                // the resized layer will be centered horizontally, so that if the image is very wide, it will extend an extra
                // amount to both the left and right, past the left and right edges of more narrow layers, equally.
                s32 total_offset = out_x - s32(Params->image_output_size_x);
                s32 offset = -total_offset / 2;

                if (layer_index == 0)
                    Log->SetTimerStart();

                // Actually resize the layer, accounting for whether the layer has an alpha channel or not.
                if (gimp_drawable_has_alpha(layer_drawable) == TRUE)
                    Thp_resize_layer_RGBA(Params, layer);
                else
                    Thp_resize_layer_RGB(Params, layer);

                if (Params->whole_image_mode == true)
                {
                    // If there is any offset to apply to the layer, now that we've resized it, apply it.
                    if (offset != 0)
                        gimp_layer_set_offsets(
                            layer,
                            (gint)offset, (gint)0
                        );
                }
                else
                {
                    // If there is any offset to apply to the layer, now that we've resized it, apply it.
                    if ((offset_old_x != 0) || (offset_old_y != 0))
                        gimp_layer_set_offsets(
                            layer,
                            (gint)offset_old_x, (gint)offset_old_y
                        );
                }

            }

            // We don't need the list of image layers anymore, so we free up memory by getting rid of it.
            if (image_layers_gfree_needed == true)
                g_free(image_layers);
            // Resize the image so that the image's size (width and height dimensions) can fit all of the layers inside of it.
            gimp_image_resize_to_layers(image);
            // If the end-user chooses to "undo" all the work we did, it does it to all the stuff before this line as one operation,
            // as explained above (earlier in the source code).
            gimp_image_undo_group_end(image);

            gimp_context_pop();
            gimp_displays_flush();
            gtk_widget_set_sensitive((GtkWidget*)Program_Dialog, (gboolean)TRUE);

            if (Combo_Size_Widget)
                delete Combo_Size_Widget;

            Params->DisengagePluginPriority();

            return true;
        }
        else if (Params->run_mode == RUN_MODE_RESIZE__KEEP_ASPECT_SAME_HORIZONTAL_V2)
        {
            // If the end-user uses the "undo" command for the image, everything from the "gimp_image_undo_group_start" function to
            // the "gimp_image_undo_group_end" function will be a single undo operation, and also show up in GIMP's undo history as
            // a single entry in a list of commands and operations on the image.
            gimp_image_undo_group_start(image);

            /*
            Params->input_size_x = Params->image_input_size_x;
            Params->input_size_y = Params->image_input_size_y;
            Params->output_size_x = Params->image_output_size_x;
            Params->output_size_y = Params->image_output_size_y;
            */

            s32 output_size_param_x = (s32)Params->output_size_x;
            s32 output_size_param_y = (s32)Params->output_size_y;

            // Iterate through (Go one-by-one, in-order, through) all of the layers of the image, and resize them.
            for (s32 layer_index = 0; layer_index < number_of_layers; layer_index++)
            {
                // Set pointers for the current image's "layer" and the layer's "drawable", as GIMP calls them.
                layer = image_layers[layer_index];
                GimpDrawable* layer_drawable = (GimpDrawable*)layer;

                // If the current layer is a text layer, convert it into a regular layer of pixels before going any further.
                if ( gimp_item_is_text_layer((GimpItem*)layer) == TRUE )
                {
                    gimp_rasterizable_rasterize((GimpRasterizable*)layer);
                }

                if (Params->whole_image_mode == false)
                {
                    gimp_drawable_get_offsets(
                        drawable,
                        &offset_old_x, &offset_old_y
                    );
                }

                // Set the current layer to have an offset of zero pixels, in both x and y dimensions.
                gimp_layer_set_offsets(
                    layer,
                    (gint)0, (gint)0
                );

                s32 out_x, out_y, layer_x, layer_y;
                f128 layer_aspect, out_y_f;

                if (Params->whole_image_mode == true)
                {
                    // Grab the values of the new height and width, or x and y dimensions, of what to resize layers to, and keep
                    // the "y" dimension, which is the height to resize all layers of the image to, the same, while maintaining
                    // the aspect ratio of all of the layers of the image, which means that the width, or the "x" dimension, can vary,
                    // and has to be recalculated for each layer independently.  The initial width or "x" dimension to resize layers to,
                    // which was chosen in the GUI by the end-user, is actually ignored and is a value that isn't used.
                    out_x = (s32)Params->image_output_size_x;
                    out_y = (s32)Params->image_output_size_y;
                    layer_x = (s32)gimp_drawable_get_width(layer_drawable);
                    layer_y = (s32)gimp_drawable_get_height(layer_drawable);
                    layer_aspect = (f128)layer_x / (f128)layer_y;
                    out_y_f = f128(out_x) / layer_aspect;
                    out_y = s32(out_y_f);
                }
                else
                {
                    out_x = output_size_param_x;
                    out_y = output_size_param_y;
                    layer_x = (s32)gimp_drawable_get_width(layer_drawable);
                    layer_y = (s32)gimp_drawable_get_height(layer_drawable);
                    layer_aspect = (f128)layer_x / (f128)layer_y;
                    out_y_f = f128(out_x) / layer_aspect;
                    out_y = s32(out_y_f);
                }

                // Update the parameters for the starting x and y dimensions, and also for the x and y dimensions to resize to,
                // for the current layer, now that we've figured out what they are.
                Params->input_size_x = (u64)layer_x;
                Params->input_size_y = (u64)layer_y;
                Params->output_size_x = (u64)out_x;
                Params->output_size_y = (u64)out_y;

                // If the GUI is enabled, update the progress percentages completed, for both the current layer, and also
                // all of the layers altogether.  Also, update the Parameter variable for which layer is being processed,
                // for the GUI display.
                if (Params->gui_enabled == true)
                {
                    f64 progress_start = f64(layer_index) / f64(number_of_layers);
                    Params->progress_start = (f64)progress_start;
                    Params->progress_end = f64(progress_start + progress_size);

                    Params->draw_index = layer_index;
                }

                // Update the paramaters with the information on what we're doing with this later of the image.
                Params->CopySizesToLayerSizes();
                Params->CalcAll();

                // If the GUI is enabled, update the GUI's display on-screen.
                if (Params->gui_enabled == true)
                    Combo_Size_Widget->SyncDataFromParameters();

                // Calculate the new offset of the y dimension, if any.  Cutting it in half will produce an end-result where
                // the resized layer will be vertically centered, so that if the image is very tall, it will extend an extra
                // amount above the top of shorter layers, and below the bottom of shorter layers, in equal amounts at the
                // top and bottom.  If this layer is shorter than other layers, it will be padded with extra space above and
                // below it, with the padding in equal amounts above and below this layer.
                s32 total_offset = out_y - s32(Params->image_output_size_y);
                s32 offset = -total_offset / 2;

                if (layer_index == 0)
                    Log->SetTimerStart();

                // Actually resize the layer, accounting for whether the layer has an alpha channel or not.
                if (gimp_drawable_has_alpha(layer_drawable) == TRUE)
                    Thp_resize_layer_RGBA(Params, layer);
                else
                    Thp_resize_layer_RGB(Params, layer);

                if (Params->whole_image_mode == true)
                {
                    // If there is any offset to apply to the layer, now that we've resized it, apply it.
                    if (offset != 0)
                        gimp_layer_set_offsets(
                            layer,
                            (gint)0, (gint)offset
                        );
                }
                else
                {
                    // If there is any offset to apply to the layer, now that we've resized it, apply it.
                    if ((offset_old_x != 0) || (offset_old_y != 0))
                        gimp_layer_set_offsets(
                            layer,
                            (gint)offset_old_x, (gint)offset_old_y
                        );
                }

            }

            // We don't need the list of image layers anymore, so we free up memory by getting rid of it.
            if (image_layers_gfree_needed == true)
                g_free(image_layers);
            // Resize the image so that the image's size (width and height dimensions) can fit all of the layers inside of it.
            gimp_image_resize_to_layers(image);
            // If the end-user chooses to "undo" all the work we did, it does it to all the stuff before this line as one operation,
            // as explained above (earlier in the source code).
            gimp_image_undo_group_end(image);

            gimp_context_pop();
            gimp_displays_flush();
            gtk_widget_set_sensitive((GtkWidget*)Program_Dialog, (gboolean)TRUE);

            if (Combo_Size_Widget)
                delete Combo_Size_Widget;

            Params->DisengagePluginPriority();

            return true;
        }

        Params->operation_result_string = "Error: Couldn't figure out what work needs to be done on the image.";
        return false;
    }
};
