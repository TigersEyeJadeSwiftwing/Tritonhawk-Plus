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

    GList *List = g_list_append(NULL, g_strdup(THP_PLUG_IN_PROCESS_Test_Internal_Functions));
    List = g_list_append(List, g_strdup(THP_PLUG_IN_PROCESS_Layer_Magic_Resize));
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

    if (g_strcmp0 (name, THP_PLUG_IN_PROCESS_Test_Internal_Functions) == 0)
    {
        procedure = gimp_image_procedure_new(
            plug_in,
            name,
            GIMP_PDB_PROC_TYPE_PLUGIN,
            thp_test_internal_functions,
            NULL,
            NULL
        );

        gimp_procedure_set_image_types(procedure, "*");
        gimp_procedure_set_sensitivity_mask(procedure, GIMP_PROCEDURE_SENSITIVE_ALWAYS);
        gimp_procedure_set_menu_label(procedure, _(THP_PLUG_IN_MENU_LABEL_Test_Internal_Functions));
        gimp_procedure_set_icon_name(procedure, GIMP_ICON_GEGL);
        gimp_procedure_add_menu_path(procedure, THP_PLUG_IN_MENU_PATH_Test_Internal_Functions);

        gimp_procedure_set_documentation(
            procedure,
            _(THP_PLUG_IN_DOCUMENTATION_BLURB_Test_Internal_Functions),
            g_strdup_printf(
                _("%s" "\n"
                  "Uses 128-bit floating-point math for processing, multi-threaded."
                  ),
                THP_PLUG_IN_DOCUMENTATION_HELP_CAPTION_Test_Internal_Functions
            ),
            THP_PLUG_IN_PROCESS_Test_Internal_Functions
        );
        gimp_procedure_set_attribution(
            procedure,
            PLUG_IN_AUTHOR,
            PLUG_IN_COPYRIGHT,
            PLUG_IN_DATE
        );

        gimp_procedure_add_double_argument(
            procedure,
            "sample-interpolation-x", "Sample Interpolation X",
            "This determines how samples are read, in-between pixels or in some cases using the nearest pixel and not "
            "bothering to interpolate between four pixels (in a 2x2 grid) when a sample is located off-center of an input "
            "pixel in the sample grid and includes some of a neighboring pixel.  If set to 0.0, it uses a quick-and-dirty "
            "\"nearest neighbor\" or \"nearest pixel\" approach as just described for every input sample in the sample grid.  "
            "When set to 1.0, which is the default, you get linear interpolation.  Setting this to 2.0 gives quadratic "
            "interpolation, 3.0 is for cubic, 4.0 for quartic, and 5.0 gives quintic interpolation.  Setting this to some "
            "value in-between whole numbers gives some type of interpolation between what you get with whole numbers.  "
            "The maximum value for x and y dimensions is 50.0, which is pretty tight interpolation.",
            (gdouble)0.0, (gdouble)50.0, (gdouble)1.0,
            G_PARAM_READWRITE
        );
        gimp_procedure_add_double_argument(
            procedure,
            "sample-interpolation-y", "Sample Interpolation Y",
            "This determines how samples are read, in-between pixels or in some cases using the nearest pixel and not "
            "bothering to interpolate between four pixels (in a 2x2 grid) when a sample is located off-center of an input "
            "pixel in the sample grid and includes some of a neighboring pixel.  If set to 0.0, it uses a quick-and-dirty "
            "\"nearest neighbor\" or \"nearest pixel\" approach as just described for every input sample in the sample grid.  "
            "When set to 1.0, which is the default, you get linear interpolation.  Setting this to 2.0 gives quadratic "
            "interpolation, 3.0 is for cubic, 4.0 for quartic, and 5.0 gives quintic interpolation.  Setting this to some "
            "value in-between whole numbers gives some type of interpolation between what you get with whole numbers.  "
            "The maximum value for x and y dimensions is 50.0, which is pretty tight interpolation.",
            (gdouble)0.0, (gdouble)50.0, (gdouble)1.0,
            G_PARAM_READWRITE
        );

        gimp_procedure_add_int_argument(
            procedure,
            "chunk-size", "Chunk size in kilopixels",
            "This affects how many thousands of pixels will be processed at once, as Tritonhawk-Plus breaks up any input "
            "image into chunks of pixels, handing them out to each processing thread, and then moving on to the next big "
            "pile of pixels, until the entire layer is done.  Setting this higher can speed things up just slightly, but "
            "use up more memory and slow down updates to the GUI's progress readout.  Results will vary, though overall "
            "changing this amount to anything other than the default, 10 (which is 10k, or 10,000), won't make much noticeable "
            "difference in performance.  When running the plug-in in a script or another plug-in, and not using the GUI, "
            "changing this from it's default will probably not make much of any difference, other than how much memory is "
            "used during the procedure.",
            (gint)1, (gint)5000, (gint)10,
            G_PARAM_READWRITE
        );
    }
    else if (g_strcmp0 (name, THP_PLUG_IN_PROCESS_Layer_Magic_Resize) == 0)
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

        gimp_procedure_add_int_argument(
            procedure,
            "new-x", "New Width",
            "The width of the new, resized image.  Set this to zero to use a percentage of the image width instead.",
            (gint)0, (gint)(65536), (gint)0,
            G_PARAM_READWRITE
        );
        gimp_procedure_add_int_argument(
            procedure,
            "new-y", "New Height",
            "The height of the new, resized image.  Set this to zero to use a percentage of the image height instead.",
            (gint)0, (gint)(65536), (gint)0,
            G_PARAM_READWRITE
        );
        gimp_procedure_add_double_argument(
            procedure,
            "new-x-percent", "New Width (Percent of original image layer width)",
            "How wide the output layers should be re-scaled to.  "
            "If the image has multiple layers and the layers are not the same size, this means that the resulting image "
            "will have all of it's layers rescaled using this percentage for width.",
            (gdouble)0.00001, (gdouble)50000.0, (gdouble)100.00,
            G_PARAM_READWRITE
        );
        gimp_procedure_add_double_argument(
            procedure,
            "new-y-percent", "New Height (Percent of original image layer height)",
            "How tall the output layers should be re-scaled to.  "
            "If the image has multiple layers and the layers are not the same size, this means that the resulting image "
            "will have all of it's layers rescaled using this percentage for height.",
            (gdouble)0.00001, (gdouble)50000.0, (gdouble)100.00,
            G_PARAM_READWRITE
        );

        gimp_procedure_add_int_argument(
            procedure,
            "threads", "number of threads to use",
            "Tritonhawk-Plus plugins are multi-threaded, using OpenMP technology.  Set this number to the number of "
            "threads you want the procedure to try to use, or zero to use the default that is in GIMP's settings.",
            (gint)0, (gint)(256), (gint)0,
            G_PARAM_READWRITE
        );
        gimp_procedure_add_boolean_argument(
            procedure,
            "background", "Use background CPU priority?",
            "By default, this procedure runs in the foreground, and can tie up the various cores for processing.  "
            "Enable this if you want this procedure to run in the background and yield to other things using your "
            "computer's CPU cores.",
            (gboolean)FALSE,
            G_PARAM_READWRITE
        );

        gimp_procedure_add_string_argument(
            procedure,
            "sample-grid-shape", "Sample Grid Shape",
            "The sample grid used for reading from the source image can be square, round, or automatically set "
            "depending on if the source dimensions are larger, equal to, or smaller than the output layers.  "
            "valid strings for this parameter are \"sqaure\", \"circle\", and \"auto\", which is the default.",
            (const gchar*)"auto",
            G_PARAM_READWRITE
        );
        gimp_procedure_add_double_argument(
            procedure,
            "sample-grid-x", "Sample Grid Width (Percent of a pixel)",
            "Controls how wide the sample grid is, in percent of a single pixel of the created image.  "
            "The sample grid is used to multi-sample the old image, and if the sampling grid is large, then "
            "for every pixel of the created image a wider range of samples are taken from the old image including "
            "nearby pixels and not just an interpolated center.  Setting this high can make the image look a bit blurry "
            "but also less blocky when enlarging an image.  " "\n"
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
            "but also less blocky when enlarging an image.  " "\n"
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
        gimp_procedure_add_double_argument(
            procedure,
            "sample-grid-weighting", "Sample Grid Weighting",
            "When set to 0.0, which is the default, all of the samples in the sample grid have an equal effect when "
            "reading the source layer, to determine the output pixel's color.  This is done by finding an average of "
            "all of the input samples, by adding up all input samples using the sample grid and dividing by the total "
            "number of samples.  By comparison, at 1.0 samples at or near the center of the sample grid have more influence "
            "on the output pixel's color than at the very edge of the sample grid.  If plotted on a graph, the weighting "
            "of the samples form a cosine bell shape, with the center of the sample grid being at the top of the bell.  "
            "When this parameter is set to something in-between 0.0 and 1.0, you get a mix of the two shapes found at 0.0 "
            "and 1.0, with a \"lerp\"-style floating point mix of the two, where 0.1 does barely any weighting and is "
            "almost a totally flat shape, and so on.",
            (gdouble)0.0, (gdouble)1.0, (gdouble)0.0,
            G_PARAM_READWRITE
        );
        gimp_procedure_add_double_argument(
            procedure,
            "sample-grid-count", "Sample Grid Count",
            "This parameter controls how many samples are in the sample grid, as a percentage of the total number "
            "of samples.  The default is 100, which is 100 percent, which is usually enough samples needed for any "
            "resizing operation where the sample grid shape is square and sample weighting isn't used (set to 0.0).  "
            "Setting this below 100 percent will speed up the operation, but skip reading certain pixels as input "
            "that is used to determine output pixels.  It can be set to zero, which still gives a 1x1 grid with one "
            "sample.  When set above 100 percent, this will resize with an increase in quality, at the expense of "
            "making the resizing operation take longer.",
            (gdouble)0.0, (gdouble)5000.0, (gdouble)100.0,
            G_PARAM_READWRITE
        );

        gimp_procedure_add_double_argument(
            procedure,
            "sample-interpolation-x", "Sample Interpolation X",
            "This determines how samples are read, in-between pixels or in some cases using the nearest pixel and not "
            "bothering to interpolate between four pixels (in a 2x2 grid) when a sample is located off-center of an input "
            "pixel in the sample grid and includes some of a neighboring pixel.  If set to 0.0, it uses a quick-and-dirty "
            "\"nearest neighbor\" or \"nearest pixel\" approach as just described for every input sample in the sample grid.  "
            "When set to 1.0, which is the default, you get linear interpolation.  Setting this to 2.0 gives quadratic "
            "interpolation, 3.0 is for cubic, 4.0 for quartic, and 5.0 gives quintic interpolation.  Setting this to some "
            "value in-between whole numbers gives some type of interpolation between what you get with whole numbers.  "
            "The maximum value for x and y dimensions is 50.0, which is pretty tight interpolation.",
            (gdouble)0.0, (gdouble)50.0, (gdouble)1.0,
            G_PARAM_READWRITE
        );
        gimp_procedure_add_double_argument(
            procedure,
            "sample-interpolation-y", "Sample Interpolation Y",
            "This determines how samples are read, in-between pixels or in some cases using the nearest pixel and not "
            "bothering to interpolate between four pixels (in a 2x2 grid) when a sample is located off-center of an input "
            "pixel in the sample grid and includes some of a neighboring pixel.  If set to 0.0, it uses a quick-and-dirty "
            "\"nearest neighbor\" or \"nearest pixel\" approach as just described for every input sample in the sample grid.  "
            "When set to 1.0, which is the default, you get linear interpolation.  Setting this to 2.0 gives quadratic "
            "interpolation, 3.0 is for cubic, 4.0 for quartic, and 5.0 gives quintic interpolation.  Setting this to some "
            "value in-between whole numbers gives some type of interpolation between what you get with whole numbers.  "
            "The maximum value for x and y dimensions is 50.0, which is pretty tight interpolation.",
            (gdouble)0.0, (gdouble)50.0, (gdouble)1.0,
            G_PARAM_READWRITE
        );

        gimp_procedure_add_int_argument(
            procedure,
            "chunk-size", "Chunk size in kilopixels",
            "This affects how many thousands of pixels will be processed at once, as Tritonhawk-Plus breaks up any input "
            "image into chunks of pixels, handing them out to each processing thread, and then moving on to the next big "
            "pile of pixels, until the entire layer is done.  Setting this higher can speed things up just slightly, but "
            "use up more memory and slow down updates to the GUI's progress readout.  Results will vary, though overall "
            "changing this amount to anything other than the default, 10 (which is 10k, or 10,000), won't make much noticeable "
            "difference in performance.  When running the plug-in in a script or another plug-in, and not using the GUI, "
            "changing this from it's default will probably not make much of any difference, other than how much memory is "
            "used during the procedure.",
            (gint)1, (gint)5000, (gint)10,
            G_PARAM_READWRITE
        );

        gimp_procedure_add_string_argument(
            procedure,
            "resize-mode", "Resize Mode",
            "This parameter is for how to resize the layers of an image, and especially important for an image with "
            "multiple layers.  The default is \"same-ratio\", which resizes all layers according to the ratio of new-to-old "
            "for width and height.  Other options are \"same-dimensions\" which resizes all layers to the same end-result "
            "width and height number of pixels, no matter what the input height and width are, and also \"same-height\", "
            "which resizes all layers to have the same height in pixels but maintains the same height-to-width ratio, which "
            "means that the width can vary, and then finally \"same-width\" which is like same-height but makes all layers "
            "have the same width and same aspect ratio that they had to begin with.  You can also use \"same-x\" and \"same-y\" "
            "for the last two modes.",
            (const gchar*)"same-ratio",
            G_PARAM_READWRITE
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

#include "tritonhawk-plus-test-internal-functions.hpp"

#include "tritonhawk-plus-layer-magical-resize.hpp"
#include "tritonhawk-plus-image-magical-resize.hpp"
// #include "tritonhawk-plus-image-magical-deflate.hpp"
// #include "tritonhawk-plus-image-magical-enchant.hpp"
#include "tritonhawk-plus-image-magical-shrink.hpp"
