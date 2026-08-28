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
#include "tritonhawk-plus.hpp"
#include "components/thp_log.hpp"
#include "components/thp_resize.hpp"
#include "components/thp_parameters.hpp"
#include "components/thp_gui-combowidget-size.hpp"
#include "components/thp_operation.hpp"

#include <algorithm>
#include <iostream>
#include <omp.h>

G_DEFINE_TYPE(ThpImageResize, thpimageresize, GIMP_TYPE_PLUG_IN)

GIMP_MAIN(THPIMAGERESIZE_TYPE)

#include <algorithm>
#include <cmath>
#include <string>

using namespace std;
using namespace TritonhawkPlus;

ThpLog* Log = 0;
ThpParams* Params = 0;

// extern static GimpValueArray*  thp_image_magic_resize_run;
// extern GimpValueArray*  thp_image_magic_deflate_run;

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
    // GList *List;

    // GList *List = g_list_append(NULL, g_strdup(PLUG_IN_PROC));

    // List = g_list_append(List, g_strdup(THP_PLUG_IN_PROCESS_Image_Magic_Resize));
    // List = g_list_append(List, g_strdup(THP_PLUG_IN_PROCESS_Image_Magic_Deflate));
    // List = g_list_append(List, g_strdup(THP_PLUG_IN_PROCESS_Image_Magic_Enchant));

    GList *List = g_list_append(NULL, g_strdup(THP_PLUG_IN_PROCESS_Layer_Magic_Resize));
    List = g_list_append(List, g_strdup(THP_PLUG_IN_PROCESS_Image_Magic_Resize));
    List = g_list_append(List, g_strdup(THP_PLUG_IN_PROCESS_Image_Magic_Shrink));

    // List = g_list_append(List, g_strdup(THP_PLUG_IN_PROCESS_Image_Magic_Deflate));
    // List = g_list_append(List, g_strdup(THP_PLUG_IN_PROCESS_Image_Magic_Enchant));

    return List;

    // return g_list_append(NULL, g_strdup(PLUG_IN_PROC));
}

static GimpProcedure* thpimageresize_create_procedure(GimpPlugIn* plug_in, const gchar* name)
{
    GimpProcedure *procedure = NULL;

    if (g_strcmp0 (name, THP_PLUG_IN_PROCESS_Layer_Magic_Resize) == 0)
    {
        procedure = gimp_image_procedure_new(
            plug_in,
            name,
            GIMP_PDB_PROC_TYPE_PLUGIN,
            thp_layer_magic_resize_run,
            NULL,
            NULL
        );

        gimp_procedure_set_image_types(procedure, "RGB,RGBA");
        gimp_procedure_set_sensitivity_mask(procedure, 0);
        gimp_procedure_set_menu_label(procedure, _(THP_PLUG_IN_MENU_LABEL_Layer_Magic_Resize));
        gimp_procedure_set_icon_name(procedure, GIMP_ICON_GEGL);
        gimp_procedure_add_menu_path(procedure, THP_PLUG_IN_MENU_PATH_Layer_Magic_Resize);

        gimp_procedure_set_documentation(
            procedure,
            _(THP_PLUG_IN_DOCUMENTATION_BLURB_Layer_Magic_Resize),
            g_strdup_printf(
                _("%s" "\n"
                  "Uses 128-bit floating-point math for processing, multi-threaded."
                  ),
                THP_PLUG_IN_DOCUMENTATION_HELP_CAPTION_Layer_Magic_Resize
            ),
            THP_PLUG_IN_PROCESS_Layer_Magic_Resize
        );
        gimp_procedure_set_attribution(
            procedure,
            PLUG_IN_AUTHOR,
            PLUG_IN_COPYRIGHT,
            PLUG_IN_DATE
        );
    }
    else if (g_strcmp0 (name, THP_PLUG_IN_PROCESS_Image_Magic_Resize) == 0)
    {
        procedure = gimp_image_procedure_new(
            plug_in,
            name,
            GIMP_PDB_PROC_TYPE_PLUGIN,
            thp_image_magic_resize_run,
            NULL,
            NULL
        );

        gimp_procedure_set_image_types(procedure, "RGB,RGBA");
        gimp_procedure_set_sensitivity_mask(procedure, 0);
        gimp_procedure_set_menu_label(procedure, _(THP_PLUG_IN_MENU_LABEL_Image_Magic_Resize));
        gimp_procedure_set_icon_name(procedure, GIMP_ICON_GEGL);
        gimp_procedure_add_menu_path(procedure, THP_PLUG_IN_MENU_PATH_Image_Magic_Resize);

        gimp_procedure_set_documentation(
            procedure,
            _(THP_PLUG_IN_DOCUMENTATION_BLURB_Image_Magic_Resize),
            g_strdup_printf(
                _("%s" "\n"
                  "Uses 128-bit floating-point math for processing, multi-threaded."
                  ),
                THP_PLUG_IN_DOCUMENTATION_HELP_CAPTION_Image_Magic_Resize
            ),
            THP_PLUG_IN_PROCESS_Image_Magic_Resize
        );
        gimp_procedure_set_attribution(
            procedure,
            PLUG_IN_AUTHOR,
            PLUG_IN_COPYRIGHT,
            PLUG_IN_DATE
        );
    }
    else if (g_strcmp0 (name, THP_PLUG_IN_PROCESS_Image_Magic_Shrink) == 0)
    {
        procedure = gimp_image_procedure_new(
            plug_in,
            name,
            GIMP_PDB_PROC_TYPE_PLUGIN,
            thp_image_magic_shrink_run,
            NULL,
            NULL
        );

        gimp_procedure_set_image_types(procedure, "RGB,RGBA");
        gimp_procedure_set_sensitivity_mask(procedure, 0);
        gimp_procedure_set_menu_label(procedure, _(THP_PLUG_IN_MENU_LABEL_Image_Magic_Shrink));
        gimp_procedure_set_icon_name(procedure, GIMP_ICON_GEGL);
        gimp_procedure_add_menu_path(procedure, THP_PLUG_IN_MENU_PATH_Image_Magic_Shrink);

        gimp_procedure_set_documentation(
            procedure,
            _(THP_PLUG_IN_DOCUMENTATION_BLURB_Image_Magic_Shrink),
            g_strdup_printf(
                _("%s" "\n"
                  "Uses 128-bit floating-point math for processing, multi-threaded."
                  ),
                THP_PLUG_IN_DOCUMENTATION_HELP_CAPTION_Image_Magic_Shrink
            ),
            THP_PLUG_IN_MENU_LABEL_Image_Magic_Shrink
        );
        gimp_procedure_set_attribution(
            procedure,
            PLUG_IN_AUTHOR,
            PLUG_IN_COPYRIGHT,
            PLUG_IN_DATE
        );
    }
    /*
    else if (g_strcmp0 (name, THP_PLUG_IN_PROCESS_Image_Magic_Deflate) == 0)
    {
        procedure = gimp_image_procedure_new(
            plug_in,
            name,
            GIMP_PDB_PROC_TYPE_PLUGIN,
            thp_image_magic_deflate_run,
            NULL,
            NULL
        );

        gimp_procedure_set_image_types(procedure, "RGB,RGBA");
        gimp_procedure_set_sensitivity_mask(procedure, 0);
        gimp_procedure_set_menu_label(procedure, _(THP_PLUG_IN_MENU_LABEL_Image_Magic_Deflate));
        gimp_procedure_set_icon_name(procedure, GIMP_ICON_GEGL);
        gimp_procedure_add_menu_path(procedure, THP_PLUG_IN_MENU_PATH_Image_Magic_Deflate);

        gimp_procedure_set_documentation(
            procedure,
            _(THP_PLUG_IN_DOCUMENTATION_BLURB_Image_Magic_Deflate),
            g_strdup_printf(
                _("%s" "\n"
                  "Uses 128-bit floating-point math for processing, multi-threaded."
                  ),
                THP_PLUG_IN_DOCUMENTATION_HELP_CAPTION_Image_Magic_Deflate
            ),
            THP_PLUG_IN_PROCESS_Image_Magic_Deflate
        );
        gimp_procedure_set_attribution(
            procedure,
            PLUG_IN_AUTHOR,
            PLUG_IN_COPYRIGHT,
            PLUG_IN_DATE
        );
    }
    else if (g_strcmp0 (name, THP_PLUG_IN_PROCESS_Image_Magic_Enchant) == 0)
    {
        procedure = gimp_image_procedure_new(
            plug_in,
            name,
            GIMP_PDB_PROC_TYPE_PLUGIN,
            thp_image_magic_enchant_run,
            NULL,
            NULL
        );

        gimp_procedure_set_image_types(procedure, "RGB,RGBA");
        gimp_procedure_set_sensitivity_mask(procedure, 0);
        gimp_procedure_set_menu_label(procedure, _(THP_PLUG_IN_MENU_LABEL_Image_Magic_Enchant));
        gimp_procedure_set_icon_name(procedure, GIMP_ICON_GEGL);
        gimp_procedure_add_menu_path(procedure, THP_PLUG_IN_MENU_PATH_Image_Magic_Enchant);

        gimp_procedure_set_documentation(
            procedure,
            _(THP_PLUG_IN_DOCUMENTATION_BLURB_Image_Magic_Enchant),
            g_strdup_printf(
                _("%s" "\n"
                  "Uses 128-bit floating-point math for processing, multi-threaded."
                  ),
                THP_PLUG_IN_DOCUMENTATION_HELP_CAPTION_Image_Magic_Enchant
            ),
            THP_PLUG_IN_PROCESS_Image_Magic_Enchant
        );
        gimp_procedure_set_attribution(
            procedure,
            PLUG_IN_AUTHOR,
            PLUG_IN_COPYRIGHT,
            PLUG_IN_DATE
        );
    }
    */
    /*
    else if (g_strcmp0 (name, PLUG_IN_PROC) == 0)
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
    */

    return procedure;
}

#include "tritonhawk-plus-layer-magical-resize.hpp"
#include "tritonhawk-plus-image-magical-resize.hpp"
// #include "tritonhawk-plus-image-magical-deflate.hpp"
// #include "tritonhawk-plus-image-magical-enchant.hpp"
#include "tritonhawk-plus-image-magical-shrink.hpp"
