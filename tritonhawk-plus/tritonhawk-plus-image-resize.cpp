#include <algorithm>

#include "components/thp_types.h"
#include "tritonhawk-plus-image-resize.h"
#include "components/thp_log.h"
#include "components/thp_resize.h"
#include "components/thp_parameters.h"
#include "components/thp_gui-combowidget-size.h"

#include <iostream>
#include <omp.h>

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

G_DEFINE_TYPE(ThpImageResize, thpimageresize, GIMP_TYPE_PLUG_IN)

GIMP_MAIN(THPIMAGERESIZE_TYPE)

#include <algorithm>
#include <cmath>
#include <string>

using namespace std;
using namespace TritonhawkPlus;

ThpLog* Log;

static void thpimageresize_class_init(ThpImageResizeClass* klass)
{
    GimpPlugInClass* plug_in_class = GIMP_PLUG_IN_CLASS(klass);

    plug_in_class->query_procedures = thpimageresize_query_procedures;
    plug_in_class->create_procedure = thpimageresize_create_procedure;
    plug_in_class->set_i18n = thpimageresize_set_i18n;
}

static void thpimageresize_init(ThpImageResize* thpimageresize)
{
}

static gboolean thpimageresize_set_i18n(
    GimpPlugIn* plug_in,
    const gchar* procedure_name,
    gchar** gettext_domain,
    gchar** catalog_dir
)
{
    return false;
}

static GList* thpimageresize_query_procedures(GimpPlugIn* plug_in)
{
  return g_list_append(NULL, g_strdup(PLUG_IN_PROC));
}

static GimpProcedure* thpimageresize_create_procedure(GimpPlugIn* plug_in, const gchar* name)
{
    GimpProcedure *procedure = NULL;

    if (g_strcmp0 (name, PLUG_IN_PROC) == 0)
    {
        procedure = gimp_image_procedure_new(
            plug_in,
            name,
            GIMP_PDB_PROC_TYPE_PLUGIN,
            thpimageresize_run,
            NULL,
            NULL
        );

        gimp_procedure_set_image_types(procedure, "RGB,RGBA");
        gimp_procedure_set_sensitivity_mask(procedure, 0);
        gimp_procedure_set_menu_label(procedure, _(PLUG_IN_MENU_LABEL));
        gimp_procedure_set_icon_name(procedure, GIMP_ICON_GEGL);
        gimp_procedure_add_menu_path(procedure, PLUG_IN_MENU_PATH);

        gimp_procedure_set_documentation(
            procedure,
            _(PLUG_IN_DOCUMENTATION_BLURB),
            g_strdup_printf(
                _("%s" "\n"
                  "Uses 128-bit floating-point math for processing, multi-threaded."
                  ),
                PLUG_IN_DOCUMENTATION_HELP_CAPTION
            ),
            PLUG_IN_PROC
        );
        gimp_procedure_set_attribution(
            procedure,
            PLUG_IN_AUTHOR,
            PLUG_IN_COPYRIGHT,
            PLUG_IN_DATE
        );
        gimp_procedure_add_int_argument(
            procedure,
            "new-x", "New Width", "The width of the new, resized image.",
            (gint)1, (gint)(65536), (gint)256,
            G_PARAM_READWRITE
        );
        gimp_procedure_add_int_argument(
            procedure,
            "new-y", "New Height", "The height of the new, resized image.",
            (gint)1, (gint)(65536), (gint)256,
            G_PARAM_READWRITE
        );
        gimp_procedure_add_double_argument(
            procedure,
            "sample-grid-x", "Sample Grid Width (Percent of a pixel)",
            "Controls how wide the sample grid is, in percent of a single pixel of the created image.  "
            "The sample grid is used to multi-sample the old image, and if the sampling grid is large, then "
            "for every pixel of the created image a wider range of samples are taken from the old image including "
            "nearby pixels and not just an interpolated center.  Setting this high can make the image look a bit blurry "
            "but also less blocky when enlarging an image.  "
            "Note that when this parameter is set higher, it often results in more total samples being taken to make the "
            "end-result image, which slows down the process and also means breaking the work up into smaller chunks at a "
            "time, which is calculated automatically.",
            (gdouble)0.01, (gdouble)5000.0, (gdouble)100.00,
            G_PARAM_READWRITE
        );
        gimp_procedure_add_double_argument(
            procedure,
            "sample-grid-y", "Sample Grid Height (Percent of a pixel)",
            "Controls how tall the sample grid is, in percent of a single pixel of the created image.  "
            "The sample grid is used to multi-sample the old image, and if the sampling grid is large, then "
            "for every pixel of the created image a wider range of samples are taken from the old image including "
            "nearby pixels and not just an interpolated center.  Setting this high can make the image look a bit blurry "
            "but also less blocky when enlarging an image.  "
            "Note that when this parameter is set higher, it often results in more total samples being taken to make the "
            "end-result image, which slows down the process and also means breaking the work up into smaller chunks at a "
            "time, which is calculated automatically.",
            (gdouble)0.01, (gdouble)5000.0, (gdouble)100.00,
            G_PARAM_READWRITE
        );
        gimp_procedure_add_boolean_argument(
            procedure,
            "seamless-x", "Seamless Horizontal Tiling",
            "Enable this if the resized image is meant to be tiled or repeated vertically, and blend nicely at the edges "
            "without any ugly seams that give away where the end of one of the tiles ends and the next one begins.  "
            "Note that this doesn't create a smooth blend if the graphic doesn't already tile nicely, but if turned on "
            "then it prevents a seam from appearing.",
            (gboolean)FALSE,
            G_PARAM_READWRITE
        );
        gimp_procedure_add_boolean_argument(
            procedure,
            "seamless-y", "Seamless Vertical Tiling",
            "Enable this if the resized image is meant to be tiled or repeated horizontally, and blend nicely at the edges "
            "without any ugly seams that give away where the end of one of the tiles ends and the next one begins.  "
            "Note that this doesn't create a smooth blend if the graphic doesn't already tile nicely, but if turned on "
            "then it prevents a seam from appearing.",
            (gboolean)FALSE,
            G_PARAM_READWRITE
        );
    }

    return procedure;
}

static GimpValueArray* thpimageresize_run(
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
    Params->process_name = (string)PLUG_IN_PROC;

    int max_threads = omp_get_max_threads();
    int pref_threads = (int)gimp_get_num_processors();
    int enabled_threads = std::min(max_threads, pref_threads);
    omp_set_num_threads(enabled_threads);

    gint new_size_x =               gint(256);
    gint new_size_y =               gint(256);
    gint old_size_x =               gint(256);
    gint old_size_y =               gint(256);
    gboolean seamless_x =           gboolean(FALSE);
    gboolean seamless_y =           gboolean(FALSE);
    gdouble sample_grid_x =         gdouble(100.0);
    gdouble sample_grid_y =         gdouble(100.0);
    gint chunk_size =               gint(256);

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
            PLUG_IN_PROC
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
    Params->chunk_size_default = (u64)chunk_size * 1024uL;
    Params->chunk_size_kilo = (u64)chunk_size;
    Params->CalcAll();

    if (run_mode == GIMP_RUN_INTERACTIVE)
    {
        GdkGeometry         geometry;

        gimp_ui_init (PLUG_IN_BINARY);
        Program_Dialog = gimp_dialog_new (
            PLUG_IN_PROC,
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

    Log->SetTimerStart();

    gimp_context_push ();
    gimp_context_set_interpolation (GIMP_INTERPOLATION_NONE);

    double progress_size = 1.0 / double(drawable_count);

    if (run_mode != GIMP_RUN_INTERACTIVE)
    {
        g_object_get (config,
            "new-x",                &new_size_x,
            "new-y",                &new_size_y,
            "sample-grid-x",        &sample_grid_x,
            "sample-grid-y",        &sample_grid_y,
            "seamless-x",           &seamless_x,
            "seamless-y",           &seamless_y,
            NULL_TERMINATE
        );

        image_copy = gimp_image_duplicate (image);
        gimp_image_scale (image, (gint)new_size_x, (gint)new_size_y);
        layer_list_image = gimp_image_list_layers(image);
        layer_list_image_copy = gimp_image_list_layers(image_copy);

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
        Params->chunk_size_default = (int)chunk_size * 1024;
        Params->CalcSampleGrid();
        Params->CalcNumberOfChunks();
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
        chunk_size = (gint)(Params->chunk_size_default / 1024);

        image_copy = gimp_image_duplicate (image);
        gimp_image_scale (image, (gint)new_size_x, (gint)new_size_y);
        layer_list_image = gimp_image_list_layers(image);
        layer_list_image_copy = gimp_image_list_layers(image_copy);

        Params->number_threads = (int)enabled_threads;
        Params->draw_count = (int)drawable_count;
        Params->draw_index = (int)0;
        Params->CalcAll();
    }

    for (int drawable_index = 0; drawable_index < drawable_count; drawable_index++)
    {
        GimpDrawable* layer_image = (GimpDrawable*)g_list_nth_data (layer_list_image, (guint)drawable_index);
        GimpDrawable* layer_image_copy = (GimpDrawable*)g_list_nth_data (layer_list_image_copy, (guint)drawable_index);

        f64 progress_start = f64(drawable_index) / f64(drawable_count);
        Params->progress_start = (f64)progress_start;
        Params->progress_end = f64(progress_start + progress_size);

        if (gimp_drawable_has_alpha(layer_image_copy) == TRUE)
            Thp_Resize_drawable_RGBA(Params, layer_image_copy, layer_image);
        else
            Thp_Resize_drawable_RGB(Params, layer_image_copy, layer_image);
    }

    g_list_free(layer_list_image);
    g_list_free(layer_list_image_copy);

    gimp_image_delete(image_copy);

    gimp_context_pop ();

    omp_set_num_threads(max_threads);

    f64 time_done_ms = Log->GetTimerElapsedMS();

    if (run_mode == GIMP_RUN_INTERACTIVE)
        gimp_displays_flush();

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

    delete Log;
    delete Params;
    if (Combo_Size_Widget) delete Combo_Size_Widget;

    return gimp_procedure_new_return_values (procedure, GIMP_PDB_SUCCESS, NULL);
};
