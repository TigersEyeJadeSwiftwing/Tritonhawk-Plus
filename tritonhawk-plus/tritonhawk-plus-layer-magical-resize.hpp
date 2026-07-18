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

#ifndef THP_PLUG_IN_LAYER_RESIZE
#define THP_PLUG_IN_LAYER_RESIZE

#include <memory>

static GimpValueArray*  thp_layer_magic_resize_run(
                            GimpProcedure* procedure,
                            GimpRunMode run_mode,
                            GimpImage* image,
                            GimpDrawable** drawables,
                            GimpProcedureConfig* config,
                            gpointer run_data
                        )
{
    Log = new ThpLog();
    ThpParams* Params = new ThpParams();
    Params->process_name = (string)THP_PLUG_IN_PROCESS_Layer_Magic_Resize;

    int max_threads = omp_get_max_threads();
    int pref_threads = (int)gimp_get_num_processors();
    int enabled_threads = std::min(max_threads, pref_threads);
    enabled_threads = max_threads;
    omp_set_num_threads(enabled_threads);

    /*
    std::shared_ptr<GtkWidget>              Program_Dialog;
    std::shared_ptr<GtkWidget>              Gui_Log_Box_0;
    std::shared_ptr<GtkWidget>              Gui_Log_Text_0;
    std::shared_ptr<GtkWidget>              Gui_Log_Text_1;
    std::shared_ptr<ComboSizeWidget>        Combo_Size_Widget;
    */

    // If there's no image to take any layers and resize, there's no work to be done, and we might as well stop
    // here, and abort with an error message.
    if (!image)
    {
        GError *error = NULL;
        g_set_error(
            &error,
            GIMP_PLUG_IN_ERROR,
            0,
            _("\n----- Procedure '%s' requires an input image.  No input image detected."),
            THP_PLUG_IN_PROCESS_Layer_Magic_Resize
        );

        delete Log;
        delete Params;

        return gimp_procedure_new_return_values(procedure, GIMP_PDB_CALLING_ERROR, error);
    }

    // Get the current, selected layers (not the same thing as visible layers), and find out how many of them there
    // are.  By the way, it is possible for a layer to be selected and not visible, which also happens.
    GimpLayer** layers_selected = gimp_image_get_selected_layers((GimpImage*)image);
    // std::shared_ptr<GimpLayer*> layers_selected = std::make_shared<GimpLayer[]>( gimp_image_get_selected_layers((GimpImage*)image) );
    s32 number_of_selected_layers = 0;
    for (int i = 0; i < 5000; i++)
    {
        if (layers_selected[i])
            number_of_selected_layers++;
        else
            break;
    }

    // Get the total number of drawables in the image.
    s32 number_of_drawables = 0;
    for (int i = 0; i < 5000; i++)
    {
        if (drawables[i])
            number_of_drawables++;
        else
            break;
    }

    // Check to see if there are no selected layers at all.  If that's the case, abort with a friendly error message,
    // politely annoying as error messages are by reputation.  I won't give you any more on this for the
    // born-yesterday-impaired.
    if (number_of_selected_layers < 1)
    {
        GError *error = NULL;
        g_set_error(
            &error,
            GIMP_PLUG_IN_ERROR,
            0,
            _("\n----- Procedure '%s' requires at least one selected layer.  Now I'm about to lie to you: Sorry."),
            THP_PLUG_IN_PROCESS_Layer_Magic_Resize
        );

        delete Log;
        delete Params;

        return gimp_procedure_new_return_values(procedure, GIMP_PDB_CALLING_ERROR, error);
    }

    // For now, this plug-in won't resize multiple layers unless its run on the entire image.  This is a start,
    // to be expanded later so that multiple selected layers can be processed.  Also, this plug-in might not handle a
    // floating selected layer, such as a copied and pasted floating selection, yet.
    if (number_of_selected_layers > 1)
    {
        GError *error = NULL;
        g_set_error(
            &error,
            GIMP_PLUG_IN_ERROR,
            0,
            _("\n----- Procedure '%s' requires one selected layer, not more.  One at a time please, because I'm stupid and incomplete, and my creator hasn't programmed the rest, for now."),
            THP_PLUG_IN_PROCESS_Layer_Magic_Resize
        );

        delete Log;
        delete Params;

        return gimp_procedure_new_return_values(procedure, GIMP_PDB_CALLING_ERROR, error);
    }

    // If we've gotten this far, we should have a single, valid layer, which is selected.  Check to see if it is floating, like what you
    // get from a copy-and-paste operation, and if it's floating, then for now we're going to be stupid, at least until more work is done.
    if ( gimp_layer_is_floating_sel((GimpLayer*)layers_selected[0]) == (gboolean)TRUE )
    {
        GError *error = NULL;
        g_set_error(
            &error,
            GIMP_PLUG_IN_ERROR,
            0,
            _("\n----- Procedure '%s' doesn't do floating selections, for now.  You'll have to wait until the plugin is finished, as if."),
            THP_PLUG_IN_PROCESS_Layer_Magic_Resize
        );

        delete Log;
        delete Params;

        return gimp_procedure_new_return_values(procedure, GIMP_PDB_CALLING_ERROR, error);
    }

    {
        // This stuff is here for now, and I'll move it when I get around to it.
        // std::shared_ptr<GimpDrawable> drawable = std::make_shared<GimpDrawable>((GimpDrawable*)layer);
        // gint image_size_x = gimp_image_get_width(image);
        // gint image_size_y = gimp_image_get_height(image);
    }

    gint layer_size_x = (gint) gimp_drawable_get_width( (GimpDrawable*)layers_selected[0] );
    gint layer_size_y = (gint) gimp_drawable_get_height( (GimpDrawable*)layers_selected[0] );
    gint offset_old_x = (gint) 0;
    gint offset_old_y = (gint) 0;
    gint offset_x = (gint) 0;
    gint offset_y = (gint) 0;

    if (number_of_selected_layers == 1)
    {
        gimp_drawable_get_offsets(
            (GimpDrawable*)layers_selected[0],
            &offset_old_x, &offset_old_y
        );

        layer_size_x -= offset_old_x;
        layer_size_y -= offset_y;
    }

    if (run_mode != GIMP_RUN_INTERACTIVE)
    {
        Params->run_mode = RUN_MODE_RESIZE__SINGLE_LAYER_V2;
        Params->hardware_max_threads = (s16)omp_get_max_threads();
        Params->preferences_max_threads = (s16)gimp_get_num_processors();
        Params->number_threads = (s16)min(Params->hardware_max_threads, Params->preferences_max_threads);
        Params->draw_count = (s32)1;
        Params->draw_index = (s32)0;
        Params->input_size_x = (u64)layer_size_x;
        Params->input_size_y = (u64)layer_size_y;
        Params->output_size_x = (u64)10uLL;
        Params->output_size_y = (u64)10uLL;
        Params->sample_grid_shape = SAMPLE_GRID_SHAPE_Square;
        Params->sample_grid_width_percent = (f64)100.0;
        Params->sample_grid_height_percent = (f64)100.0;
        Params->chunk_size_default = (u64)5000uLL;
        Params->chunk_size_kilo = (u64)10uLL;
        Params->sample_interpolation_x = 0.0_q;
        Params->sample_interpolation_y = 0.0_q;
        Params->CalcAll();

        if ( (Params->input_size_x == (u64)10uLL) && (Params->input_size_y == (u64)10uLL) )
        {
            Params->output_size_x = (u64)1uLL;
            Params->output_size_y = (u64)1uLL;
        }

        gimp_context_push ();
        gimp_context_set_interpolation (GIMP_INTERPOLATION_NONE);
        // f64 progress_size = 1.0 / f64 (number_of_selected_layers);

        if (number_of_selected_layers == 1)
        {
            // If the end-user uses the "undo" command for the image, everything from the "gimp_image_undo_group_start" function to
            // the "gimp_image_undo_group_end" function will be a single undo operation, and also show up in GIMP's undo history as
            // a single entry in a list of commands and operations on the image.
            gimp_image_undo_group_start(image);

            // Create a C++ pointer to the first layer of the array of selected layers, for convenience and to make typing and reading this source
            // code a little easier.  Ditto for the drawable of the layer.
            GimpLayer* layer = (GimpLayer*)layers_selected[0];
            GimpDrawable* drawable = (GimpDrawable*)layer;

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

                offset_x = (gint)(f128( (f128)offset_old_x * (f128)Params->output_size_x / (f128)Params->input_size_x ));
                offset_y = (gint)(f128( (f128)offset_old_y * (f128)Params->output_size_y / (f128)Params->input_size_y ));
            }

            // Update the paramaters with the information on what we're doing with this later of the image.
            Params->CalcSampleGrid();
            Params->CalcNumberOfChunks();
            Params->CalcAll();

            // Actually resize the layer, accounting for whether the layer has an alpha channel or not.
            if (gimp_drawable_has_alpha(drawable) == TRUE)
                Thp_resize_layer_RGBA(Params, layer);
            else
                Thp_resize_layer_RGB(Params, layer);

            if (number_of_drawables == 1)
                gimp_image_resize(
                    image,
                    (gint)Params->output_size_x, (gint)Params->output_size_y,
                    (gint)0, (gint)0
                );
            else if ( (offset_x != (gint)0) || (offset_y != (gint)0) )
                gimp_layer_set_offsets(
                    layer,
                    (gint)offset_x, (gint)offset_y
                );

            // If the end-user chooses to "undo" all the work we did, it does it to all the stuff before this line as one operation,
            // as explained above (earlier in the source code).
            gimp_image_undo_group_end(image);
        }

        gimp_context_pop();

        delete Log;
        delete Params;

        g_free(layers_selected);

        return gimp_procedure_new_return_values (procedure, GIMP_PDB_SUCCESS, NULL);
    }

    // if (run_mode == GIMP_RUN_INTERACTIVE).....

    Params->run_mode = RUN_MODE_RESIZE__SINGLE_LAYER_V2;
    Params->layers_to_process = LAYERS_TO_PROCESS__ACTIVE_LAYER;
    Params->hardware_max_threads = (s16)omp_get_max_threads();
    Params->preferences_max_threads = (s16)gimp_get_num_processors();
    Params->number_threads = Params->hardware_max_threads;
    Params->draw_count = (s32)1;
    Params->draw_index = (s32)0;
    Params->input_size_x = (u64)layer_size_x;
    Params->input_size_y = (u64)layer_size_y;
    Params->output_size_x = Params->input_size_x;
    Params->output_size_y = Params->input_size_y;
    Params->chunk_size_default = (u64)5000uLL;
    Params->chunk_size_kilo = (u64)10uLL;
    Params->CalcAll();

    GtkWidget*              Program_Dialog;
    GtkWidget*              Gui_Log_Box_0;
    GtkWidget*              Gui_Log_Text_0;
    GtkWidget*              Gui_Log_Text_1;
    ComboSizeWidget*        Combo_Size_Widget;
    GdkGeometry             geometry;

    gimp_ui_init (PLUG_IN_BINARY);
    Program_Dialog = gimp_dialog_new (
        THP_PLUG_IN_PROCESS_Layer_Magic_Resize,
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
        delete Log;
        delete Params;
        if (Combo_Size_Widget) delete Combo_Size_Widget;

        return gimp_procedure_new_return_values (procedure, GIMP_PDB_CANCEL, NULL);
    }

    gimp_context_push ();

    Params->EngagePluginPriority();
    Log->SetTimerStart();
    gtk_widget_set_sensitive((GtkWidget*)Program_Dialog, (gboolean)FALSE);

    gimp_context_set_interpolation (GIMP_INTERPOLATION_NONE);
    // f64 progress_size = 0.0;

    if (number_of_selected_layers == 1)
    {
        // If the end-user uses the "undo" command for the image, everything from the "gimp_image_undo_group_start" function to
        // the "gimp_image_undo_group_end" function will be a single undo operation, and also show up in GIMP's undo history as
        // a single entry in a list of commands and operations on the image.
        gimp_image_undo_group_start(image);

        // Create a C++ pointer to the first layer of the array of selected layers, for convenience and to make typing and reading this source
        // code a little easier.  Ditto for the drawable of the layer.
        GimpLayer* layer = (GimpLayer*)layers_selected[0];
        GimpDrawable* drawable = (GimpDrawable*)layer;

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

            offset_x = (gint)(f128( (f128)offset_old_x * (f128)Params->output_size_x / (f128)Params->input_size_x ));
            offset_y = (gint)(f128( (f128)offset_old_y * (f128)Params->output_size_y / (f128)Params->input_size_y ));
        }

        // Update the paramaters with the information on what we're doing with this later of the image.
        Params->draw_index = 0;
        Params->CalcSampleGrid();
        Params->CalcNumberOfChunks();
        Params->CalcAll();

        Combo_Size_Widget->SyncDataFromParameters();

        // Actually resize the layer, accounting for whether the layer has an alpha channel or not.
        if (gimp_drawable_has_alpha(drawable) == TRUE)
            Thp_resize_layer_RGBA(Params, layer);
        else
            Thp_resize_layer_RGB(Params, layer);

        if (number_of_drawables == 1)
            gimp_image_resize(
                image,
                (gint)Params->output_size_x, (gint)Params->output_size_y,
                (gint)0, (gint)0
            );
        else if ( (offset_x != (gint)0) || (offset_y != (gint)0) )
            gimp_layer_set_offsets(
                layer,
                (gint)offset_x, (gint)offset_y
            );

        // If the end-user chooses to "undo" all the work we did, it does it to all the stuff before this line as one operation,
        // as explained above (earlier in the source code).
        gimp_image_undo_group_end(image);
    }

    gimp_context_pop();
    gimp_displays_flush();
    gtk_widget_set_sensitive((GtkWidget*)Program_Dialog, (gboolean)TRUE);
    Params->DisengagePluginPriority();

    delete Log;
    delete Params;
    if (Combo_Size_Widget)
        delete Combo_Size_Widget;

    g_free(layers_selected);

    return gimp_procedure_new_return_values (procedure, GIMP_PDB_SUCCESS, NULL);

/*
        // If the end-user uses the "undo" command for the image, everything from the "gimp_image_undo_group_start" function to
        // the "gimp_image_undo_group_end" function will be a single undo operation, and also show up in GIMP's undo history as
        // a single entry in a list of commands and operations on the image.
        gimp_image_undo_group_start(image);

        // Iterate through (Go one-by-one, in-order, through) all of the layers of the image, and resize them.
        for (s32 layer_index = 0; layer_index < number_of_selected_layers; layer_index++)
        {
            // Set pointers for the current image's "layer" and the layer's "drawable", as GIMP calls them.
            GimpLayer* layer = (GimpLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
            GimpDrawable* layer_drawable = (GimpDrawable*)layer;

            // If the current layer is a text layer, convert it into a regular layer of pixels before going any further.
            if ( gimp_item_is_text_layer((GimpItem*)layer) == TRUE )
            {
                GimpTextLayer* layer_text = (GimpTextLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
                gimp_rasterizable_rasterize((GimpRasterizable*)layer_text);
                layer = (GimpLayer*)layer_text;
                layer_drawable = (GimpDrawable*)layer;
            }

            // If the GUI is enabled, update the progress percentages completed, for both the current layer, and also
            // all of the layers altogether.  Also, update the Parameter variable for which layer is being processed,
            // for the GUI display.
            if (Params->gui_enabled == true)
            {
                f64 progress_start = f64(layer_index) / f64(number_of_selected_layers);
                Params->progress_start = (f64)progress_start;
                Params->progress_end = f64(progress_start + progress_size);

                Params->draw_index = layer_index;
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
            Params->CalcSampleGrid();
            Params->CalcNumberOfChunks();
            Params->CalcAll();

            // If the GUI is enabled, update the GUI's display on-screen.
            if (Params->gui_enabled == true)
                Combo_Size_Widget->SyncDataFromParameters();

            // Actually resize the layer, accounting for whether the layer has an alpha channel or not.
            if (gimp_drawable_has_alpha(layer_drawable) == TRUE)
                Thp_resize_layer_RGBA(Params, layer);
            else
                Thp_resize_layer_RGB(Params, layer);

        } // END of looping through all layers of the image.

        // We don't need the list of image layers anymore, so we free up memory by getting rid of it.
        g_list_free(image_layer_list);
        // Resize the image so that the image's size (width and height dimensions) can fit all of the layers inside of it.
        gimp_image_resize_to_layers(image);
        // If the end-user chooses to "undo" all the work we did, it does it to all the stuff before this line as one operation,
        // as explained above (earlier in the source code).
        gimp_image_undo_group_end(image);
    }

    // g_free(layers_selected);
    */
}

#endif // THP_PLUG_IN_LAYER_RESIZE
