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

#ifndef THP_PLUG_IN_ENCHANT
#define THP_PLUG_IN_ENCHANT

static GimpValueArray*  thp_image_magic_enchant_run(
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
    Params->process_name = (string)THP_PLUG_IN_PROCESS_Image_Magic_Enchant;

    int max_threads = omp_get_max_threads();
    int pref_threads = (int)gimp_get_num_processors();
    int enabled_threads = std::min(max_threads, pref_threads);
    omp_set_num_threads(enabled_threads);

    gint new_size_x =               gint(1);
    gint new_size_y =               gint(1);
    gint old_size_x =               gint(256);
    gint old_size_y =               gint(256);
    gboolean seamless_x =           gboolean(FALSE);
    gboolean seamless_y =           gboolean(FALSE);
    gdouble sample_grid_x =         gdouble(100.0);
    gdouble sample_grid_y =         gdouble(100.0);
    gint chunk_size =               gint(5);

    GtkWidget*              Program_Dialog;
    GtkWidget*              Gui_Log_Box_0;
    GtkWidget*              Gui_Log_Text_0;
    GtkWidget*              Gui_Log_Text_1;
    ComboSizeWidget*        Combo_Size_Widget;

    if (!image)
    {
        GError *error = NULL;
        g_set_error(
            &error,
            GIMP_PLUG_IN_ERROR,
            0,
            _("\n----- Procedure '%s' requires an input image.  No input image detected."),
            THP_PLUG_IN_PROCESS_Image_Magic_Enchant
        );

        delete Log;
        delete Params;

        return gimp_procedure_new_return_values(procedure, GIMP_PDB_CALLING_ERROR, error);
    }

    old_size_x = gimp_image_get_width(image);
    old_size_y = gimp_image_get_height(image);

    s32 drawable_count = 0;
    GList* layer_list_check = gimp_image_list_layers(image);
    drawable_count = (s32) g_list_length(layer_list_check);
    g_list_free(layer_list_check);

    GList* layer_list_image = NULL;
    GList* layer_list_image_copy = NULL;
    GimpImage* image_copy = NULL;

    Params->run_mode = RUN_MODE_RESIZE__ALL_LAYERS_SAME_RATIO_V2;
    Params->hardware_max_threads = (s16)max_threads;
    Params->preferences_max_threads = (s16)pref_threads;
    Params->number_threads = (s16)enabled_threads;
    Params->draw_count = (s32)drawable_count;
    Params->draw_index = (s32)0;
    Params->input_size_x = (u64)old_size_x;
    Params->input_size_y = (u64)old_size_y;
    Params->output_size_x = (u64)new_size_x;
    Params->output_size_y = (u64)new_size_y;
    Params->seamless_x = (bool)seamless_x;
    Params->seamless_y = (bool)seamless_y;
    Params->sample_grid_width_percent = (f64)sample_grid_x;
    Params->sample_grid_height_percent = (f64)sample_grid_y;
    Params->chunk_size_default = (u64)chunk_size * 1000uL;
    Params->chunk_size_kilo = (u64)chunk_size;
    Params->CalcAll();

    if (run_mode == GIMP_RUN_INTERACTIVE)
    {
        GdkGeometry         geometry;

        gimp_ui_init (PLUG_IN_BINARY);
        Program_Dialog = gimp_dialog_new (
            THP_PLUG_IN_PROCESS_Image_Magic_Enchant,
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
        Params->output_size_x = Params->input_size_x;
        Params->output_size_y = Params->input_size_y;
        Params->sample_grid_shape = SAMPLE_GRID_SHAPE_Square;
        Params->sample_interpolation_x = 0.0q;
        Params->sample_interpolation_y = 0.0q;
        Combo_Size_Widget->SyncDataFromParameters();

        gtk_window_set_default_size(GTK_WINDOW (Program_Dialog), -1, -1);

        // Log->Log(false, g_strdup_printf( _("%s%s"), Params->info_string.c_str(), "-\n-\n-\n-\n-\n-\n-" ));
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
    }

    if (run_mode == GIMP_RUN_INTERACTIVE)
        gtk_widget_set_sensitive((GtkWidget*)Program_Dialog, (gboolean)FALSE);

    if (run_mode == GIMP_RUN_INTERACTIVE)
        Params->plugin_priority_realtime = true;
    else
        Params->plugin_priority_realtime = false;

    Params->EngagePluginPriority();

    if (run_mode == GIMP_RUN_INTERACTIVE)
        Log->SetTimerStart();

    // gimp_image_undo_group_start(image);
    Params->layer_is_full_frame = true;
    gimp_context_push ();
    gimp_context_set_interpolation (GIMP_INTERPOLATION_NONE);
    // GimpMetadata* image_metadata = gimp_image_get_metadata(image_old);

    double progress_size = 1.0 / double(drawable_count);

    if (run_mode != GIMP_RUN_INTERACTIVE)
    {
        /*
        g_object_get (config,
            "new-x",                &new_size_x,
            "new-y",                &new_size_y,
            "sample-grid-x",        &sample_grid_x,
            "sample-grid-y",        &sample_grid_y,
            "seamless-x",           &seamless_x,
            "seamless-y",           &seamless_y,
            NULL_TERMINATE
        );

        Params->run_mode = RUN_MODE_RESIZE__ALL_LAYERS_SAME_RATIO_V2;
        Params->number_threads = (int)enabled_threads;
        Params->draw_count = (int)drawable_count;
        Params->draw_index = (int)0;
        Params->input_size_x = (int)old_size_x;
        Params->input_size_y = (int)old_size_y;
        Params->output_size_x = (int)new_size_x;
        Params->output_size_y = (int)new_size_y;
        Params->seamless_x = (bool)seamless_x;
        Params->seamless_y = (bool)seamless_y;
        Params->sample_grid_width_percent = (f64)sample_grid_x;
        Params->sample_grid_height_percent = (f64)sample_grid_y;
        Params->chunk_size_default = (int)chunk_size * 1000;
        */

        new_size_x =        (gint) old_size_x;
        new_size_y =        (gint) old_size_y;
        seamless_x =        gboolean(FALSE);
        seamless_y =        gboolean(FALSE);
        sample_grid_x =     gdouble(100.0);
        sample_grid_y =     gdouble(100.0);
        chunk_size =        gint(5);

        Params->run_mode = RUN_MODE_RESIZE__ALL_LAYERS_SAME_RATIO_V2;
        Params->number_threads = (s16)enabled_threads;
        Params->draw_count = (s32)drawable_count;
        Params->draw_index = (s32)0;
        Params->input_size_x = (u64)old_size_x;
        Params->input_size_y = (u64)old_size_y;
        Params->output_size_x = (u64)old_size_x;
        Params->output_size_y = (u64)old_size_y;
        Params->seamless_x = false;
        Params->seamless_y = false;
        Params->sample_count_adjustment = 1.0q;
        Params->sample_grid_width_percent = 100.0;
        Params->sample_grid_height_percent = 100.0;
        Params->image_ratio_x = 1.0q;
        Params->image_ratio_y = 1.0q;
        Params->sample_grid_scale_x = 1.0q;
        Params->sample_grid_scale_y = 1.0q;
        Params->sample_interpolation_x = 0.0q;
        Params->sample_interpolation_y = 0.0q;
        Params->sample_grid_shape = SAMPLE_GRID_SHAPE_Square;
        Params->sample_grid_shape_x = SAMPLE_GRID_SHAPE_Square;
        Params->sample_grid_shape_y = SAMPLE_GRID_SHAPE_Square;
        Params->sample_grid_weighting = 0.q;
        Params->chunk_size_kilo = (u64)5uL;
        Params->chunk_size_default = (u64)5000uL;
        Params->layer_is_full_frame = true;
        Params->gui_enabled = false;

        Params->CalcSampleGrid();
        Params->CalcNumberOfChunks();
        Params->CalcAll();
    }
    else
    {
        new_size_x = (gint) Params->output_size_x;
        new_size_y = (gint) Params->output_size_y;
        old_size_x = (gint) Params->input_size_x;
        old_size_y = (gint) Params->input_size_y;
        seamless_x = (Params->seamless_x == true) ? TRUE : FALSE;
        seamless_y = (Params->seamless_y == true) ? TRUE : FALSE;
        sample_grid_x = (gdouble)Params->sample_grid_width_percent;
        sample_grid_y = (gdouble)Params->sample_grid_height_percent;
        chunk_size = (gint)(Params->chunk_size_default / 1000);

        // Params->number_threads = (int)enabled_threads;
        Params->draw_count = (int)drawable_count;
        Params->draw_index = (int)0;
        Params->CalcAll();
    }

    if (Params->run_mode == RUN_MODE_RESIZE__BASIC_OLD)
    {
        image_copy = gimp_image_duplicate (image);
        layer_list_image = gimp_image_list_layers(image);
        layer_list_image_copy = gimp_image_list_layers(image_copy);

        for (int layer_index = 0; layer_index < drawable_count; layer_index++)
        {
            GimpLayer* layer = (GimpLayer*)g_list_nth_data(layer_list_image, (guint)layer_index);
            GimpLayer* layer_copy = (GimpLayer*)g_list_nth_data(layer_list_image_copy, (guint)layer_index);
            GimpDrawable* layer_drawable = (GimpDrawable*)layer;
            GimpDrawable* layer_drawable_copy = (GimpDrawable*)layer_copy;

            s32 full_x = gimp_drawable_get_width((GimpDrawable*)layer_copy);
            s32 full_y = gimp_drawable_get_height((GimpDrawable*)layer_copy);
            s32 out_x = Params->output_size_x;
            s32 out_y = Params->output_size_y;
            if ((full_x != (s32)Params->input_size_x) || (full_y != (s32)Params->input_size_y))
            {
                f128 full_x_f128 = f128(full_x) * Params->image_ratio_x;
                f128 full_y_f128 = f128(full_y) * Params->image_ratio_y;
                out_x = s32(full_x_f128);
                out_y = s32(full_y_f128);
            }

            f64 progress_start = f64(layer_index) / f64(drawable_count);
            Params->progress_start = (f64)progress_start;
            Params->progress_end = f64(progress_start + progress_size);
            Params->CalcAll();

            gimp_layer_scale(layer, out_x, out_y, TRUE);

            if (gimp_drawable_has_alpha(layer_drawable_copy) == TRUE)
                Thp_Resize_drawable_RGBA(Params, layer_drawable_copy, layer_drawable);
            else
                Thp_Resize_drawable_RGB(Params, layer_drawable_copy, layer_drawable);

            Params->draw_index++;
        }

        gimp_image_resize_to_layers(image);
    }
    else if (Params->run_mode == RUN_MODE_RESIZE__ALL_LAYERS_SAME_DIMENSIONS_OLD)
    {
        image_copy = gimp_image_duplicate (image);
        layer_list_image = gimp_image_list_layers(image);
        layer_list_image_copy = gimp_image_list_layers(image_copy);

        for (int layer_index = 0; layer_index < drawable_count; layer_index++)
        {
            GimpLayer* layer = (GimpLayer*)g_list_nth_data(layer_list_image, (guint)layer_index);
            GimpLayer* layer_copy = (GimpLayer*)g_list_nth_data(layer_list_image_copy, (guint)layer_index);
            GimpDrawable* layer_drawable = (GimpDrawable*)layer;
            GimpDrawable* layer_drawable_copy = (GimpDrawable*)layer_copy;

            gimp_layer_set_offsets(layer, (gint)0, (gint)0);
            gimp_layer_set_offsets(layer_copy, (gint)0, (gint)0);
            gimp_layer_scale(layer, (gint)new_size_x, (gint)new_size_y, TRUE);
            gimp_layer_set_offsets(layer, (gint)0, (gint)0);
            gimp_layer_set_offsets(layer_copy, (gint)0, (gint)0);

            f64 progress_start = f64(layer_index) / f64(drawable_count);
            Params->progress_start = (f64)progress_start;
            Params->progress_end = f64(progress_start + progress_size);

            Params->draw_index = (s32)layer_index + 1;
            Params->input_size_x = (u64)gimp_drawable_get_width(layer_drawable_copy);
            Params->input_size_y = (u64)gimp_drawable_get_height(layer_drawable_copy);
            Params->output_size_x = (u64)new_size_x;
            Params->output_size_y = (u64)new_size_y;
            Params->CalcSampleGrid();
            Params->CalcNumberOfChunks();
            Params->CalcAll();

            if (gimp_drawable_has_alpha(layer_drawable_copy) == TRUE)
                Thp_Resize_drawable_RGBA(Params, layer_drawable_copy, layer_drawable);
            else
                Thp_Resize_drawable_RGB(Params, layer_drawable_copy, layer_drawable);

            Params->draw_index++;
        }

        gimp_image_resize_to_layers(image);
    }
    else if (Params->run_mode == RUN_MODE_RESIZE__KEEP_ASPECT_SAME_VERTICAL_OLD)
    {
        image_copy = gimp_image_duplicate (image);
        layer_list_image = gimp_image_list_layers(image);
        layer_list_image_copy = gimp_image_list_layers(image_copy);

        for (int layer_index = 0; layer_index < drawable_count; layer_index++)
        {
            GimpLayer* layer = (GimpLayer*)g_list_nth_data(layer_list_image, (guint)layer_index);
            GimpLayer* layer_copy = (GimpLayer*)g_list_nth_data(layer_list_image_copy, (guint)layer_index);
            GimpDrawable* layer_drawable = (GimpDrawable*)layer;
            GimpDrawable* layer_drawable_copy = (GimpDrawable*)layer_copy;

            gimp_layer_set_offsets(layer_copy, (gint)0, (gint)0);

            s32 layer_x = gimp_drawable_get_width((GimpDrawable*)layer_copy);
            s32 layer_y = gimp_drawable_get_height((GimpDrawable*)layer_copy);
            s32 out_x = (s32)Params->output_size_x;
            s32 out_y = (s32)Params->output_size_y;

            f128 layer_aspect = (f128)layer_x / (f128)layer_y;
            f128 out_x_f = f128(out_y) * layer_aspect;
            out_x = s32(out_x_f);

            gimp_layer_set_offsets(layer, (gint)0, (gint)0);
            gimp_layer_scale(layer, (gint)out_x, (gint)out_y, FALSE);
            gimp_layer_set_offsets(layer, (gint)0, (gint)0);

            f64 progress_start = f64(layer_index) / f64(drawable_count);
            Params->progress_start = (f64)progress_start;
            Params->progress_end = f64(progress_start + progress_size);

            Params->draw_index = (s32)layer_index + 1;
            Params->input_size_x = (u64)gimp_drawable_get_width(layer_drawable_copy);
            Params->input_size_y = (u64)gimp_drawable_get_height(layer_drawable_copy);
            Params->output_size_x = (u64)out_x;
            Params->output_size_y = (u64)out_y;
            Params->CalcSampleGrid();
            Params->CalcNumberOfChunks();
            Params->CalcAll();

            s32 total_offset = 0;
            s32 offset = 0;
            total_offset = out_x - s32(new_size_x);
            offset = (-total_offset / 2);

            if (gimp_drawable_has_alpha(layer_drawable_copy) == TRUE)
                Thp_Resize_drawable_RGBA(Params, layer_drawable_copy, layer_drawable);
            else
                Thp_Resize_drawable_RGB(Params, layer_drawable_copy, layer_drawable);

            gimp_layer_set_offsets(layer, (gint)offset, (gint)0);

            Params->draw_index++;
        }

        gimp_image_resize_to_layers(image);
    }
    else if (Params->run_mode == RUN_MODE_RESIZE__BASIC)
    {
        GimpImage* image_old = gimp_image_duplicate (image);
        GimpImage* image_new = image;

        GList* image_layer_list = gimp_image_list_layers(image_new);
        GList* image_old_layer_list = gimp_image_list_layers(image_old);
        s32 layer_count = (s32) drawable_count;

        for (s32 layer_index = 0; layer_index < layer_count; layer_index++)
        {
            GimpLayer* layer = (GimpLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
            GimpLayer* layer_old = (GimpLayer*)g_list_nth_data(image_old_layer_list, (guint)layer_index);
            GimpDrawable* layer_old_drawable = (GimpDrawable*)layer_old;

            if ( gimp_item_is_text_layer((GimpItem*)g_list_nth_data(image_old_layer_list, (guint)layer_index)) == TRUE )
            {
                GimpTextLayer* layer_text = (GimpTextLayer*)g_list_nth_data(image_old_layer_list, (guint)layer_index);
                gimp_rasterizable_rasterize((GimpRasterizable*)layer_text);
                layer_old = (GimpLayer*)layer_text;
                layer_old_drawable = (GimpDrawable*)layer_old;
            }

            GimpImageType layer_old_type = GIMP_RGBA_IMAGE;
            if ( ((s32)gimp_drawable_get_width(layer_old_drawable) == (s32)Params->input_size_x) && ((s32)gimp_drawable_get_height(layer_old_drawable) == (s32)Params->input_size_y) )
            {
                Params->layer_is_full_frame = true;
                layer_old_type = gimp_drawable_type(layer_old_drawable);
            }
            else
            {
                /*
                gint offset_x = 0;
                gint offset_y = 0;
                gimp_drawable_get_offsets(layer_old_drawable, &offset_x, &offset_y);

                Params->in_frame_size_x = (u64)gimp_drawable_get_width(layer_old_drawable);
                Params->in_frame_size_y = (u64)gimp_drawable_get_height(layer_old_drawable);
                Params->in_frame_min_x = (u64)offset_x;
                Params->in_frame_min_y = (u64)offset_y;
                Params->in_frame_max_x = (u64)offset_x + Params->in_frame_size_x;
                Params->in_frame_max_y = (u64)offset_y + Params->in_frame_size_y;

                // f128 out_frame_size_x_f = ceilq((f128)Params->in_frame_size_x * (f128)Params->image_ratio_x) + 0.1q;
                // f128 out_frame_size_y_f = ceilq((f128)Params->in_frame_size_y * (f128)Params->image_ratio_y) + 0.1q;
                f128 out_frame_min_x_f = f128((f128)Params->out_frame_min_x * (f128)Params->image_ratio_x);
                f128 out_frame_max_x_f = ceilq((f128)Params->out_frame_max_x * (f128)Params->image_ratio_x) + 0.1q;
                f128 out_frame_min_y_f = f128((f128)Params->out_frame_min_y * (f128)Params->image_ratio_y);
                f128 out_frame_max_y_f = ceilq((f128)Params->out_frame_max_y * (f128)Params->image_ratio_y) + 0.1q;

                Params->out_frame_min_x = clamp( (u64)out_frame_min_x_f, 0uL, Params->output_size_x );
                Params->out_frame_max_x = clamp( (u64)out_frame_max_x_f, 0uL, Params->output_size_x );
                Params->out_frame_min_y = clamp( (u64)out_frame_min_y_f, 0uL, Params->output_size_y );
                Params->out_frame_max_y = clamp( (u64)out_frame_max_y_f, 0uL, Params->output_size_y );
                Params->out_frame_size_x = Params->out_frame_max_x - Params->out_frame_min_x;
                Params->out_frame_size_y = Params->out_frame_max_y - Params->out_frame_min_y;
                */

                Params->layer_is_full_frame = true;
                gimp_layer_add_alpha(layer_old);
                gimp_layer_resize_to_image_size(layer_old);
            }

            gimp_image_remove_layer(image_new, layer);
            GimpLayer* layer_new = gimp_layer_new(
                image_new,
                gimp_item_get_name((GimpItem*)layer_old),
                (gint)Params->output_size_x,
                (gint)Params->output_size_y,
                layer_old_type,
                gimp_layer_get_opacity(layer_old),
                gimp_layer_get_mode(layer_old)
            );
            gimp_image_insert_layer(
                image_new,
                layer_new,
                NULL,
                (gint)(layer_index)
            );
            GimpDrawable* layer_new_drawable = (GimpDrawable*)layer_new;

            f64 progress_start = f64(layer_index) / f64(layer_count);
            Params->progress_start = (f64)progress_start;
            Params->progress_end = f64(progress_start + progress_size);

            Params->draw_index = layer_index;
            Params->CalcSampleGrid();
            Params->CalcNumberOfChunks();
            Params->CalcAll();

            if (gimp_drawable_has_alpha(layer_old_drawable) == TRUE)
                Thp_Resize_drawable_RGBA(Params, layer_old_drawable, layer_new_drawable);
            else
                Thp_Resize_drawable_RGB(Params, layer_old_drawable, layer_new_drawable);

            if (Params->layer_is_full_frame == false)
            {
                gimp_image_autocrop(image_new, layer_new_drawable);
            }
        }

        g_list_free(image_layer_list);
        g_list_free(image_old_layer_list);
        gimp_image_delete(image_old);

        gimp_image_resize_to_layers(image);
    }
    else if (Params->run_mode == RUN_MODE_RESIZE__ALL_LAYERS_SAME_RATIO_V2)
    {
        gimp_image_undo_group_start(image);

        GList* image_layer_list = gimp_image_list_layers(image);
        s32 layer_count = (s32) drawable_count;

        f128 scale_x_f = f128(Params->output_size_x) / f128(Params->input_size_x);
        f128 scale_y_f = f128(Params->output_size_y) / f128(Params->input_size_y);

        for (s32 layer_index = 0; layer_index < layer_count; layer_index++)
        {
            GimpLayer* layer = (GimpLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
            GimpDrawable* layer_drawable = (GimpDrawable*)layer;

            if ( gimp_item_is_text_layer((GimpItem*)layer) == TRUE )
            {
                GimpTextLayer* layer_text = (GimpTextLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
                gimp_rasterizable_rasterize((GimpRasterizable*)layer_text);
                layer = (GimpLayer*)layer_text;
                layer_drawable = (GimpDrawable*)layer;
            }

            f64 progress_start = f64(layer_index) / f64(layer_count);
            Params->progress_start = (f64)progress_start;
            Params->progress_end = f64(progress_start + progress_size);

            Params->draw_index = layer_index;

            old_size_x = (gint) gimp_drawable_get_width(layer_drawable);
            old_size_y = (gint) gimp_drawable_get_height(layer_drawable);
            f128 new_size_x_f = f128(old_size_x) * scale_x_f;
            f128 new_size_y_f = f128(old_size_y) * scale_y_f;
            new_size_x = (gint) roundq(new_size_x_f);
            new_size_y = (gint) roundq(new_size_y_f);

            Params->input_size_x = (u64)old_size_x;
            Params->input_size_y = (u64)old_size_y;
            Params->output_size_x = (u64)new_size_x;
            Params->output_size_y = (u64)new_size_y;

            Params->in_frame_min_x = 0;
            Params->in_frame_min_y = 0;
            Params->in_frame_max_x = Params->input_size_x;
            Params->in_frame_max_y = Params->input_size_y;
            Params->in_frame_size_x = Params->input_size_x;
            Params->in_frame_size_y = Params->input_size_y;

            Params->out_frame_min_x = 0;
            Params->out_frame_min_y = 0;
            Params->out_frame_max_x = Params->output_size_x;
            Params->out_frame_max_y = Params->output_size_y;
            Params->out_frame_size_x = Params->output_size_x;
            Params->out_frame_size_y = Params->output_size_y;

            Params->layer_is_full_frame = true;

            Params->CalcSampleGrid();
            Params->CalcNumberOfChunks();
            Params->CalcAll();

            if (Params->gui_enabled == true)
            {
                Combo_Size_Widget->SetOriginalSize( (gint)Params->input_size_x, (gint)Params->input_size_y );
                Combo_Size_Widget->SetSizeX( (gint)new_size_x );
                Combo_Size_Widget->SetSizeY( (gint)new_size_y );
            }

            if (gimp_drawable_has_alpha(layer_drawable) == TRUE)
                Thp_Resize_drawable_RGBA(Params, layer_drawable, layer_drawable);
            else
                Thp_Resize_drawable_RGB(Params, layer_drawable, layer_drawable);
        }

        g_list_free(image_layer_list);
        gimp_image_resize_to_layers(image);
        gimp_image_undo_group_end(image);
    }
    else if (Params->run_mode == RUN_MODE_RESIZE__ALL_LAYERS_SAME_DIMENSIONS)
    {
        GimpImage* image_old = gimp_image_duplicate (image);
        GimpImage* image_new = image;

        GList* image_layer_list = gimp_image_list_layers(image_new);
        GList* image_old_layer_list = gimp_image_list_layers(image_old);
        s32 layer_count = (s32) drawable_count;

        for (s32 layer_index = 0; layer_index < layer_count; layer_index++)
        {
            GimpLayer* layer = (GimpLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
            GimpLayer* layer_old = (GimpLayer*)g_list_nth_data(image_old_layer_list, (guint)layer_index);
            GimpDrawable* layer_old_drawable = (GimpDrawable*)layer_old;

            if ( gimp_item_is_text_layer((GimpItem*)g_list_nth_data(image_layer_list, (guint)layer_index)) == TRUE )
            {
                GimpTextLayer* layer_text = (GimpTextLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
                gimp_rasterizable_rasterize((GimpRasterizable*)layer_text);
                layer_old = (GimpLayer*)layer_text;
                layer_old_drawable = (GimpDrawable*)layer_old;
            }

            gimp_layer_set_offsets(
                layer_old,
                (gint)0,
                (gint)0
            );

            gimp_image_remove_layer(image_new, layer);
            GimpLayer* layer_new = gimp_layer_new(
                image_new,
                gimp_item_get_name((GimpItem*)layer_old),
                (gint)Params->output_size_x,
                (gint)Params->output_size_y,
                gimp_drawable_type(layer_old_drawable),
                gimp_layer_get_opacity(layer_old),
                gimp_layer_get_mode(layer_old)
            );
            gimp_image_insert_layer(
                image_new,
                layer_new,
                NULL,
                (gint)layer_index
            );
            GimpDrawable* layer_new_drawable = (GimpDrawable*)layer_new;

            f64 progress_start = f64(layer_index) / f64(layer_count);
            Params->progress_start = (f64)progress_start;
            Params->progress_end = f64(progress_start + progress_size);

            Params->draw_index = layer_index;

            Params->input_size_x = (u64)gimp_drawable_get_width(layer_old_drawable);
            Params->input_size_y = (u64)gimp_drawable_get_height(layer_old_drawable);
            Params->in_frame_min_x = 0;
            Params->in_frame_min_y = 0;
            Params->in_frame_max_x = Params->input_size_x;
            Params->in_frame_max_y = Params->input_size_y;
            Params->in_frame_size_x = Params->input_size_x;
            Params->in_frame_size_y = Params->input_size_y;

            Params->out_frame_min_x = 0;
            Params->out_frame_min_y = 0;
            Params->out_frame_max_x = Params->output_size_x;
            Params->out_frame_max_y = Params->output_size_y;
            Params->out_frame_size_x = Params->output_size_x;
            Params->out_frame_size_y = Params->output_size_y;

            Params->layer_is_full_frame = true;

            Params->CalcSampleGrid();
            Params->CalcNumberOfChunks();
            Params->CalcAll();

            if (gimp_drawable_has_alpha(layer_old_drawable) == TRUE)
                Thp_Resize_drawable_RGBA(Params, layer_old_drawable, layer_new_drawable);
            else
                Thp_Resize_drawable_RGB(Params, layer_old_drawable, layer_new_drawable);
        }

        g_list_free(image_layer_list);
        g_list_free(image_old_layer_list);
        gimp_image_delete(image_old);

        gimp_image_resize_to_layers(image);
    }
    else if (Params->run_mode == RUN_MODE_RESIZE__ALL_LAYERS_SAME_DIMENSIONS_V2)
    {
        // GimpImage* image_old = gimp_image_duplicate (image);
        // GimpImage* image_new = image;

        gimp_image_undo_group_start(image);

        GList* image_layer_list = gimp_image_list_layers(image);
        // GList* image_old_layer_list = gimp_image_list_layers(image_old);
        s32 layer_count = (s32) drawable_count;

        for (s32 layer_index = 0; layer_index < layer_count; layer_index++)
        {
            GimpLayer* layer = (GimpLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
            GimpDrawable* layer_drawable = (GimpDrawable*)layer;
            // GimpLayer* layer_old = (GimpLayer*)g_list_nth_data(image_old_layer_list, (guint)layer_index);
            // GimpDrawable* layer_old_drawable = (GimpDrawable*)layer_old;

            if ( gimp_item_is_text_layer((GimpItem*)layer) == TRUE )
            {
                GimpTextLayer* layer_text = (GimpTextLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
                gimp_rasterizable_rasterize((GimpRasterizable*)layer_text);
                layer = (GimpLayer*)layer_text;
                layer_drawable = (GimpDrawable*)layer;
            }

            gimp_layer_set_offsets(
                layer,
                (gint)0,
                (gint)0
            );

            f64 progress_start = f64(layer_index) / f64(layer_count);
            Params->progress_start = (f64)progress_start;
            Params->progress_end = f64(progress_start + progress_size);

            Params->draw_index = layer_index;

            Params->input_size_x = (u64)gimp_drawable_get_width(layer_drawable);
            Params->input_size_y = (u64)gimp_drawable_get_height(layer_drawable);
            Params->in_frame_min_x = 0;
            Params->in_frame_min_y = 0;
            Params->in_frame_max_x = Params->input_size_x;
            Params->in_frame_max_y = Params->input_size_y;
            Params->in_frame_size_x = Params->input_size_x;
            Params->in_frame_size_y = Params->input_size_y;

            Params->out_frame_min_x = 0;
            Params->out_frame_min_y = 0;
            Params->out_frame_max_x = Params->output_size_x;
            Params->out_frame_max_y = Params->output_size_y;
            Params->out_frame_size_x = Params->output_size_x;
            Params->out_frame_size_y = Params->output_size_y;

            Params->layer_is_full_frame = true;

            Params->CalcSampleGrid();
            Params->CalcNumberOfChunks();
            Params->CalcAll();

            if (Params->gui_enabled == true)
            {
                Combo_Size_Widget->SetOriginalSize( (gint)Params->input_size_x, (gint)Params->input_size_y );
                Combo_Size_Widget->SetSizeX( (gint)new_size_x );
                Combo_Size_Widget->SetSizeY( (gint)new_size_y );
            }

            if (gimp_drawable_has_alpha(layer_drawable) == TRUE)
                Thp_Resize_drawable_RGBA(Params, layer_drawable, layer_drawable);
            else
                Thp_Resize_drawable_RGB(Params, layer_drawable, layer_drawable);
        }

        g_list_free(image_layer_list);
        // g_list_free(image_old_layer_list);
        // gimp_image_delete(image_old);

        gimp_image_resize_to_layers(image);

        gimp_image_undo_group_end(image);
    }
    else if (Params->run_mode == RUN_MODE_RESIZE__KEEP_ASPECT_SAME_VERTICAL)
    {
        GimpImage* image_old = gimp_image_duplicate (image);
        GimpImage* image_new = image;

        GList* image_layer_list = gimp_image_list_layers(image_new);
        GList* image_old_layer_list = gimp_image_list_layers(image_old);
        s32 layer_count = (s32) drawable_count;

        for (s32 layer_index = 0; layer_index < layer_count; layer_index++)
        {
            GimpLayer* layer = (GimpLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
            GimpLayer* layer_old = (GimpLayer*)g_list_nth_data(image_old_layer_list, (guint)layer_index);
            GimpDrawable* layer_old_drawable = (GimpDrawable*)layer_old;

            if ( gimp_item_is_text_layer((GimpItem*)g_list_nth_data(image_layer_list, (guint)layer_index)) == TRUE )
            {
                GimpTextLayer* layer_text = (GimpTextLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
                gimp_rasterizable_rasterize((GimpRasterizable*)layer_text);
                layer_old = (GimpLayer*)layer_text;
                layer_old_drawable = (GimpDrawable*)layer_old;
            }

            gimp_layer_set_offsets(
                layer_old,
                (gint)0,
                (gint)0
            );

            s32 layer_x = gimp_drawable_get_width(layer_old_drawable);
            s32 layer_y = gimp_drawable_get_height(layer_old_drawable);
            s32 out_x = (s32)Params->output_size_x;
            s32 out_y = (s32)Params->output_size_y;

            f128 layer_aspect = (f128)layer_x / (f128)layer_y;
            f128 out_x_f = f128(out_y) * layer_aspect;
            out_x = s32(out_x_f);

            gimp_image_remove_layer(image_new, layer);
            GimpLayer* layer_new = gimp_layer_new(
                image_new,
                gimp_item_get_name((GimpItem*)layer_old),
                (gint)out_x,
                (gint)out_y,
                gimp_drawable_type(layer_old_drawable),
                gimp_layer_get_opacity(layer_old),
                gimp_layer_get_mode(layer_old)
            );
            gimp_image_insert_layer(
                image_new,
                layer_new,
                NULL,
                (gint)layer_index
            );
            GimpDrawable* layer_new_drawable = (GimpDrawable*)layer_new;

            f64 progress_start = f64(layer_index) / f64(layer_count);
            Params->progress_start = (f64)progress_start;
            Params->progress_end = f64(progress_start + progress_size);

            Params->draw_index = layer_index;

            Params->input_size_x = (u64)gimp_drawable_get_width(layer_old_drawable);
            Params->input_size_y = (u64)gimp_drawable_get_height(layer_old_drawable);
            Params->in_frame_min_x = 0;
            Params->in_frame_min_y = 0;
            Params->in_frame_max_x = Params->input_size_x;
            Params->in_frame_max_y = Params->input_size_y;
            Params->in_frame_size_x = Params->input_size_x;
            Params->in_frame_size_y = Params->input_size_y;

            Params->output_size_x = (u64)out_x;
            Params->output_size_y = (u64)out_y;
            Params->out_frame_min_x = 0;
            Params->out_frame_min_y = 0;
            Params->out_frame_max_x = (u64)out_x;
            Params->out_frame_max_y = (u64)out_y;
            Params->out_frame_size_x = (u64)out_x;
            Params->out_frame_size_y = (u64)out_y;

            Params->layer_is_full_frame = true;

            /*
            Params->input_size_x = (u64)gimp_drawable_get_width(layer_old_drawable);
            Params->input_size_y = (u64)gimp_drawable_get_height(layer_old_drawable);
            Params->output_size_x = (u64)out_x;
            Params->output_size_y = (u64)out_y;
            */

            Params->CalcSampleGrid();
            Params->CalcNumberOfChunks();
            Params->CalcAll();

            s32 total_offset = 0;
            s32 offset = 0;
            total_offset = out_x - s32(new_size_x);
            offset = (-total_offset / 2);

            if (gimp_drawable_has_alpha(layer_old_drawable) == TRUE)
                Thp_Resize_drawable_RGBA(Params, layer_old_drawable, layer_new_drawable);
            else
                Thp_Resize_drawable_RGB(Params, layer_old_drawable, layer_new_drawable);

            gimp_layer_set_offsets(layer_new, (gint)offset, (gint)0);
        }

        g_list_free(image_layer_list);
        g_list_free(image_old_layer_list);
        gimp_image_delete(image_old);

        gimp_image_resize_to_layers(image);
    }
    else if (Params->run_mode == RUN_MODE_RESIZE__KEEP_ASPECT_SAME_VERTICAL_V2)
    {
        gimp_image_undo_group_start(image);

        GList* image_layer_list = gimp_image_list_layers(image);
        s32 layer_count = (s32) drawable_count;

        for (s32 layer_index = 0; layer_index < layer_count; layer_index++)
        {
            GimpLayer* layer = (GimpLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
            GimpDrawable* layer_drawable = (GimpDrawable*)layer;

            if ( gimp_item_is_text_layer((GimpItem*)layer) == TRUE )
            {
                GimpTextLayer* layer_text = (GimpTextLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
                gimp_rasterizable_rasterize((GimpRasterizable*)layer_text);
                layer = (GimpLayer*)layer_text;
                layer_drawable = (GimpDrawable*)layer;
            }

            gimp_layer_set_offsets(
                layer,
                (gint)0,
                (gint)0
            );

            s32 layer_x = gimp_drawable_get_width(layer_drawable);
            s32 layer_y = gimp_drawable_get_height(layer_drawable);
            s32 out_x = (s32)Params->output_size_x;
            s32 out_y = (s32)Params->output_size_y;

            f128 layer_aspect = (f128)layer_x / (f128)layer_y;
            f128 out_x_f = f128(out_y) * layer_aspect;
            out_x = s32(out_x_f);

            f64 progress_start = f64(layer_index) / f64(layer_count);
            Params->progress_start = (f64)progress_start;
            Params->progress_end = f64(progress_start + progress_size);

            Params->draw_index = layer_index;

            Params->input_size_x = (u64)gimp_drawable_get_width(layer_drawable);
            Params->input_size_y = (u64)gimp_drawable_get_height(layer_drawable);
            Params->in_frame_min_x = 0;
            Params->in_frame_min_y = 0;
            Params->in_frame_max_x = Params->input_size_x;
            Params->in_frame_max_y = Params->input_size_y;
            Params->in_frame_size_x = Params->input_size_x;
            Params->in_frame_size_y = Params->input_size_y;

            Params->output_size_x = (u64)out_x;
            Params->output_size_y = (u64)out_y;
            Params->out_frame_min_x = 0;
            Params->out_frame_min_y = 0;
            Params->out_frame_max_x = (u64)out_x;
            Params->out_frame_max_y = (u64)out_y;
            Params->out_frame_size_x = (u64)out_x;
            Params->out_frame_size_y = (u64)out_y;

            Params->layer_is_full_frame = true;

            Params->CalcSampleGrid();
            Params->CalcNumberOfChunks();
            Params->CalcAll();

            if (Params->gui_enabled == true)
            {
                Combo_Size_Widget->SetOriginalSize( (gint)layer_x, (gint)layer_y );
                Combo_Size_Widget->SetSizeX( (gint)out_x );
                Combo_Size_Widget->SetSizeY( (gint)out_y );
            }

            s32 total_offset = 0;
            s32 offset = 0;
            total_offset = out_x - s32(new_size_x);
            offset = (-total_offset / 2);

            if (gimp_drawable_has_alpha(layer_drawable) == TRUE)
                Thp_Resize_drawable_RGBA(Params, layer_drawable, layer_drawable);
            else
                Thp_Resize_drawable_RGB(Params, layer_drawable, layer_drawable);

            gimp_layer_set_offsets(layer, (gint)offset, (gint)0);
        }

        g_list_free(image_layer_list);

        gimp_image_resize_to_layers(image);

        gimp_image_undo_group_end(image);
    }
    else if (Params->run_mode == RUN_MODE_RESIZE__KEEP_ASPECT_SAME_HORIZONTAL)
    {
        GimpImage* image_old = gimp_image_duplicate (image);
        GimpImage* image_new = image;

        GList* image_layer_list = gimp_image_list_layers(image_new);
        GList* image_old_layer_list = gimp_image_list_layers(image_old);
        s32 layer_count = (s32) drawable_count;

        for (s32 layer_index = 0; layer_index < layer_count; layer_index++)
        {
            GimpLayer* layer = (GimpLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
            GimpLayer* layer_old = (GimpLayer*)g_list_nth_data(image_old_layer_list, (guint)layer_index);
            GimpDrawable* layer_old_drawable = (GimpDrawable*)layer_old;

            if ( gimp_item_is_text_layer((GimpItem*)g_list_nth_data(image_layer_list, (guint)layer_index)) == TRUE )
            {
                GimpTextLayer* layer_text = (GimpTextLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
                gimp_rasterizable_rasterize((GimpRasterizable*)layer_text);
                layer_old = (GimpLayer*)layer_text;
                layer_old_drawable = (GimpDrawable*)layer_old;
            }

            gimp_layer_set_offsets(
                layer_old,
                (gint)0,
                (gint)0
            );

            s32 layer_x = gimp_drawable_get_width(layer_old_drawable);
            s32 layer_y = gimp_drawable_get_height(layer_old_drawable);
            s32 out_x = (s32)Params->output_size_x;
            s32 out_y = (s32)Params->output_size_y;

            f128 layer_aspect = (f128)layer_x / (f128)layer_y;
            f128 out_y_f = f128(out_x) / layer_aspect;
            out_y = s32(out_y_f);

            gimp_image_remove_layer(image_new, layer);
            GimpLayer* layer_new = gimp_layer_new(
                image_new,
                gimp_item_get_name((GimpItem*)layer_old),
                (gint)out_x,
                (gint)out_y,
                gimp_drawable_type(layer_old_drawable),
                gimp_layer_get_opacity(layer_old),
                gimp_layer_get_mode(layer_old)
            );
            gimp_image_insert_layer(
                image_new,
                layer_new,
                NULL,
                (gint)layer_index
            );
            GimpDrawable* layer_new_drawable = (GimpDrawable*)layer_new;

            f64 progress_start = f64(layer_index) / f64(layer_count);
            Params->progress_start = (f64)progress_start;
            Params->progress_end = f64(progress_start + progress_size);

            Params->draw_index = layer_index;

            /*
            Params->input_size_x = (u64)gimp_drawable_get_width(layer_old_drawable);
            Params->input_size_y = (u64)gimp_drawable_get_height(layer_old_drawable);
            Params->output_size_x = (u64)out_x;
            Params->output_size_y = (u64)out_y;
            */

            Params->input_size_x = (u64)gimp_drawable_get_width(layer_old_drawable);
            Params->input_size_y = (u64)gimp_drawable_get_height(layer_old_drawable);
            Params->in_frame_min_x = 0;
            Params->in_frame_min_y = 0;
            Params->in_frame_max_x = Params->input_size_x;
            Params->in_frame_max_y = Params->input_size_y;
            Params->in_frame_size_x = Params->input_size_x;
            Params->in_frame_size_y = Params->input_size_y;

            Params->output_size_x = (u64)out_x;
            Params->output_size_y = (u64)out_y;
            Params->out_frame_min_x = 0;
            Params->out_frame_min_y = 0;
            Params->out_frame_max_x = Params->output_size_x;
            Params->out_frame_max_y = Params->output_size_y;
            Params->out_frame_size_x = Params->output_size_x;
            Params->out_frame_size_y = Params->output_size_y;

            Params->layer_is_full_frame = true;

            Params->CalcSampleGrid();
            Params->CalcNumberOfChunks();
            Params->CalcAll();

            s32 total_offset = 0;
            s32 offset = 0;
            total_offset = out_y - s32(new_size_y);
            offset = (-total_offset / 2);

            if (gimp_drawable_has_alpha(layer_old_drawable) == TRUE)
                Thp_Resize_drawable_RGBA(Params, layer_old_drawable, layer_new_drawable);
            else
                Thp_Resize_drawable_RGB(Params, layer_old_drawable, layer_new_drawable);

            gimp_layer_set_offsets(layer_new, (gint)0, (gint)offset);
        }

        g_list_free(image_layer_list);
        g_list_free(image_old_layer_list);
        gimp_image_delete(image_old);

        gimp_image_resize_to_layers(image);
    }
    else if (Params->run_mode == RUN_MODE_RESIZE__KEEP_ASPECT_SAME_HORIZONTAL_V2)
    {
        gimp_image_undo_group_start(image);

        GList* image_layer_list = gimp_image_list_layers(image);
        s32 layer_count = (s32) drawable_count;

        for (s32 layer_index = 0; layer_index < layer_count; layer_index++)
        {
            GimpLayer* layer = (GimpLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
            GimpDrawable* layer_drawable = (GimpDrawable*)layer;

            if ( gimp_item_is_text_layer((GimpItem*)layer) == TRUE )
            {
                GimpTextLayer* layer_text = (GimpTextLayer*)g_list_nth_data(image_layer_list, (guint)layer_index);
                gimp_rasterizable_rasterize((GimpRasterizable*)layer_text);
                layer = (GimpLayer*)layer_text;
                layer_drawable = (GimpDrawable*)layer;
            }

            gimp_layer_set_offsets(
                layer,
                (gint)0,
                (gint)0
            );

            s32 layer_x = gimp_drawable_get_width(layer_drawable);
            s32 layer_y = gimp_drawable_get_height(layer_drawable);
            s32 out_x = (s32)Params->output_size_x;
            s32 out_y = (s32)Params->output_size_y;

            f128 layer_aspect = (f128)layer_x / (f128)layer_y;
            f128 out_y_f = f128(out_x) / layer_aspect;
            out_y = s32(out_y_f);

            f64 progress_start = f64(layer_index) / f64(layer_count);
            Params->progress_start = (f64)progress_start;
            Params->progress_end = f64(progress_start + progress_size);

            Params->draw_index = layer_index;

            Params->input_size_x = (u64)gimp_drawable_get_width(layer_drawable);
            Params->input_size_y = (u64)gimp_drawable_get_height(layer_drawable);
            Params->in_frame_min_x = 0;
            Params->in_frame_min_y = 0;
            Params->in_frame_max_x = Params->input_size_x;
            Params->in_frame_max_y = Params->input_size_y;
            Params->in_frame_size_x = Params->input_size_x;
            Params->in_frame_size_y = Params->input_size_y;

            Params->output_size_x = (u64)out_x;
            Params->output_size_y = (u64)out_y;
            Params->out_frame_min_x = 0;
            Params->out_frame_min_y = 0;
            Params->out_frame_max_x = (u64)out_x;
            Params->out_frame_max_y = (u64)out_y;
            Params->out_frame_size_x = (u64)out_x;
            Params->out_frame_size_y = (u64)out_y;

            Params->layer_is_full_frame = true;

            Params->CalcSampleGrid();
            Params->CalcNumberOfChunks();
            Params->CalcAll();

            if (Params->gui_enabled == true)
            {
                Combo_Size_Widget->SetOriginalSize( (gint)layer_x, (gint)layer_y );
                Combo_Size_Widget->SetSizeX( (gint)out_x );
                Combo_Size_Widget->SetSizeY( (gint)out_y );
            }

            s32 total_offset = 0;
            s32 offset = 0;
            total_offset = out_y - s32(new_size_y);
            offset = (-total_offset / 2);

            if (gimp_drawable_has_alpha(layer_drawable) == TRUE)
                Thp_Resize_drawable_RGBA(Params, layer_drawable, layer_drawable);
            else
                Thp_Resize_drawable_RGB(Params, layer_drawable, layer_drawable);

            gimp_layer_set_offsets(layer, (gint)0, (gint)offset);
        }

        g_list_free(image_layer_list);

        gimp_image_resize_to_layers(image);

        gimp_image_undo_group_end(image);
    }

    gimp_context_pop ();
    // gimp_image_undo_group_end(image);

    omp_set_num_threads(max_threads);

    f64 time_done_ms = 0.0;

    if (run_mode == GIMP_RUN_INTERACTIVE)
        time_done_ms = Log->GetTimerElapsedMS();

    if (run_mode == GIMP_RUN_INTERACTIVE)
        gimp_displays_flush();

    if (run_mode == GIMP_RUN_INTERACTIVE)
        Log->Log(false, g_strdup_printf( _("%s"
            "-" "\n"
            "-     All done, finished in %11.5lf seconds." "\n"
            "-     Log Finished" "\n"
            "%s"
        ),
        Params->info_string.c_str(),
        time_done_ms * 0.001,
        "-\n-\n-\n-" ));

    if (run_mode == GIMP_RUN_INTERACTIVE)
        gtk_widget_set_sensitive((GtkWidget*)Program_Dialog, (gboolean)TRUE);

    Params->DisengagePluginPriority();

    delete Log;
    delete Params;

    if (run_mode == GIMP_RUN_INTERACTIVE)
    {
        if (Combo_Size_Widget) delete Combo_Size_Widget;
    }

    if (run_mode == GIMP_RUN_INTERACTIVE)
        return gimp_procedure_new_return_values (procedure, GIMP_PDB_SUCCESS, NULL);
    else
        return gimp_procedure_new_return_values (procedure, GIMP_PDB_SUCCESS, NULL);
};

#endif // THP_PLUG_IN_ENCHANT
