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

#pragma once
#ifndef THP_PLUG_IN_MAIN
#define THP_PLUG_IN_MAIN

/*
PLUG_IN_BINARY="(gchar*)(\"tritonhawk-plus-image-resize\")"
PLUG_IN_SOURCE="(gchar*)(\"tritonhawk-plus-image-resize.cpp\")"
PLUG_IN_PROC="(gchar*)(\"tritonhawk-plus-image-resize\")"
PLUG_IN_ROLE="(gchar*)(\"tritonhawk-plus-image-resize\")"
PLUG_IN_MENU_LABEL="(gchar*)(\"Tritonhawk Plus Multi-Threaded Resize\")"
PLUG_IN_MENU_PATH="(gchar*)(\"<Image>/Image/Tritonhawk Plus/\")"
PLUG_IN_DOCUMENTATION_BLURB="(gchar*)(\"Tritonhawk Plus Multi-Threaded Image Resize\")"
PLUG_IN_DOCUMENTATION_HELP_CAPTION="(gchar*)(\"Tritonhawk Plus Multi-Threaded Image Resize\")"
PLUG_IN_AUTHOR="(gchar*)(\"Tiger J. Swiftwing\")"
PLUG_IN_COPYRIGHT="(gchar*)(\"(c) 2025 Tiger J. Swiftwing\")"
PLUG_IN_DATE="(gchar*)(\"July 2025 CE\")"
PLUG_IN_GUI_DIALOG_CAPTION="(gchar*)(\"Tritonhawk Plus Multi-Threaded Image Resize\")"

PLUG_IN_BINARY="(gchar*)(\"tritonhawk-plus\")"
PLUG_IN_SOURCE="(gchar*)(\"tritonhawk-plus.*\")"
PLUG_IN_PROCCESS_Image_Resize="(gchar*)(\"tritonhawk-plus\")"
PLUG_IN_ROLE="(gchar*)(\"tritonhawk-plus\")"
PLUG_IN_MENU_LABEL_Image_Resize="(gchar*)(\"Tritonhawk Plus Multi-Threaded Image Resize\")"
PLUG_IN_MENU_PATH_Image_Resize="(gchar*)(\"<Image>/Image/Tritonhawk Plus/\")"
PLUG_IN_DOCUMENTATION_BLURB_Image_Resize="(gchar*)(\"Tritonhawk Plus Multi-Threaded Image Resize\")"
PLUG_IN_DOCUMENTATION_HELP_CAPTION_Image_Resize="(gchar*)(\"Tritonhawk Plus Multi-Threaded Image Resize\")"
PLUG_IN_AUTHOR="(gchar*)(\"Tiger J. Swiftwing, as David A. Reeves, SS# 287-74-8719\")"
PLUG_IN_COPYRIGHT="(gchar*)(\"(c) 2025-2026 Tiger J. Swiftwing, All Rights Reserved.\")"
PLUG_IN_DATE="(gchar*)(\"July 2025 CE - July 2026 CE\")"
PLUG_IN_GUI_DIALOG_CAPTION_Image_Resize="(gchar*)(\"Tritonhawk Plus Multi-Threaded Image Resize\")"
*/

#define THP_PLUG_IN_BINARY "tritonhawk-plus"
#define THP_PLUG_IN_ROLE "tritonhawk-plus-enchanted"

#define THP_PLUG_IN_PROCESS_Image_Magic_Resize                                  "tritonhawk-plus-enchanted-image-magical-resize"
#define THP_PLUG_IN_MENU_LABEL_Image_Magic_Resize                               "Tritonhawk Plus Image Magical Resize"
#define THP_PLUG_IN_GUI_DIALOG_CAPTION_Image_Magic_Resize                       "Tritonhawk Plus Image Magical Resize"
#define THP_PLUG_IN_MENU_PATH_Image_Magic_Resize                                "<Image>/Image/Tritonhawk Plus/"
#define THP_PLUG_IN_DOCUMENTATION_BLURB_Image_Magic_Resize                      "Tritonhawk Plus Image Magical Resize"
#define THP_PLUG_IN_DOCUMENTATION_HELP_CAPTION_Image_Magic_Resize               "Tritonhawk Plus Image Magical Resize"

#define THP_PLUG_IN_PROCESS_Layer_Magic_Resize                                  "tritonhawk-plus-enchanted-layer-magical-resize"
#define THP_PLUG_IN_MENU_LABEL_Layer_Magic_Resize                               "Tritonhawk Plus Layer Magical Resize"
#define THP_PLUG_IN_GUI_DIALOG_CAPTION_Layer_Magic_Resize                       "Tritonhawk Plus Layer Magical Resize"
#define THP_PLUG_IN_MENU_PATH_Layer_Magic_Resize                                "<Image>/Layer/Tritonhawk Plus/"
#define THP_PLUG_IN_DOCUMENTATION_BLURB_Layer_Magic_Resize                      "Tritonhawk Plus Layer Magical Resize"
#define THP_PLUG_IN_DOCUMENTATION_HELP_CAPTION_Layer_Magic_Resize               "Tritonhawk Plus Layer Magical Resize"

#define THP_PLUG_IN_PROCESS_Image_Magic_Deflate                                 "tritonhawk-plus-enchanted-image-magical-deflate"
#define THP_PLUG_IN_MENU_LABEL_Image_Magic_Deflate                              "Tritonhawk Plus Image Magical Deflate"
#define THP_PLUG_IN_GUI_DIALOG_CAPTION_Image_Magic_Deflate                      "Tritonhawk Plus Image Magical Deflate"
#define THP_PLUG_IN_MENU_PATH_Image_Magic_Deflate                               "<Image>/Image/Tritonhawk Plus/"
#define THP_PLUG_IN_DOCUMENTATION_BLURB_Image_Magic_Deflate                     "Tritonhawk Plus Image Magical Deflate"
#define THP_PLUG_IN_DOCUMENTATION_HELP_CAPTION_Image_Magic_Deflate              "Tritonhawk Plus Image Magical Deflate"

#define THP_PLUG_IN_PROCESS_Image_Magic_Enchant                                 "tritonhawk-plus-enchanted-image-enchant"
#define THP_PLUG_IN_MENU_LABEL_Image_Magic_Enchant                              "Tritonhawk Plus Image Enchantment"
#define THP_PLUG_IN_MENU_LABEL_Image_Magic_Enchant                              "Tritonhawk Plus Image Enchantment"
#define THP_PLUG_IN_GUI_DIALOG_CAPTION_Image_Magic_Enchant                      "Tritonhawk Plus Image Magical Enchantment"
#define THP_PLUG_IN_MENU_PATH_Image_Magic_Enchant                               "<Image>/Image/Tritonhawk Plus/"
#define THP_PLUG_IN_DOCUMENTATION_BLURB_Image_Magic_Enchant                     "Tritonhawk Plus Image Magical Cleaning and Enchantment"
#define THP_PLUG_IN_DOCUMENTATION_HELP_CAPTION_Image_Magic_Enchant              "Tritonhawk Plus Image Magical Enchantment"

#define THP_PLUG_IN_PROCESS_Image_Magic_Metadata_Strip                          "tritonhawk-plus-enchanted-image-metadata-strip"
#define THP_PLUG_IN_MENU_LABEL_Image_Magic_Metadata_Strip                       "Tritonhawk Plus Strip Metadata MENU_LABEL"
#define THP_PLUG_IN_GUI_DIALOG_CAPTION_Image_Magic_Metadata_Strip               "Tritonhawk Plus Strip Metadata GUI_CAPTION"
#define THP_PLUG_IN_MENU_PATH_Image_Magic_Metadata_Strip                        "<Image>/Image/Tritonhawk Plus/"
#define THP_PLUG_IN_DOCUMENTATION_BLURB_Image_Magic_Metadata_Strip              "Tritonhawk Plus Strip Metadata BLURB"
#define THP_PLUG_IN_DOCUMENTATION_HELP_CAPTION_Image_Magic_Metadata_Strip       "Tritonhawk Plus Strip Metadata HELP_CAPTION"

#define THP_PLUG_IN_AUTHOR "Tiger J. Swiftwing, as David A. Reeves, SS# 287-74-8719"
#define THP_PLUG_IN_COPYRIGHT "(c) 2025-2026 Tiger J. Swiftwing, All Rights Reserved."
#define THP_PLUG_IN_DATE "July 2025 CE - July 2026 CE"

/*
#include "config_h/clang64/config.h"

#ifndef GIMP_DISABLE_COMPAR_CRUFT
#define GIMP_DISABLE_COMPAR_CRUFT
#endif

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
*/

// #include <omp.h>
#include "components/thp_types.hpp"
// #include <string>

using namespace std;
// using namespace TritonhawkPlus;

typedef struct _ThpImageResize ThpImageResize;
typedef struct _ThpImageResizeClass ThpImageResizeClass;

struct _ThpImageResize
{
    GimpPlugIn parent_instance;
};

struct _ThpImageResizeClass
{
    GimpPlugInClass parent_class;
};

#define THPIMAGERESIZE_TYPE (thpimageresize_get_type())
#define THPIMAGERESIZE(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), THPIMAGERESIZE_TYPE, ThpImageResize))

GType                   thpimageresize_get_type(void) G_GNUC_CONST;
static void             thpimageresize_class_init(
                            ThpImageResizeClass* klass
                        );
static void             thpimageresize_init(
                            ThpImageResize* thpimageresize
                        );
static GList*           thpimageresize_query_procedures(
                            GimpPlugIn* plug_in
                        );
static GimpProcedure*   thpimageresize_create_procedure(
                            GimpPlugIn* plug_in,
                            const gchar* name
                        );
static gboolean         thpimageresize_set_i18n(
                            GimpPlugIn* plug_in,
                            const gchar* procedure_name,
                            gchar** gettext_domain,
                            gchar** catalog_dir
                        );
/*
static GimpValueArray*  thpimageresize_run(
                            GimpProcedure* procedure,
                            GimpRunMode run_mode,
                            GimpImage* image,
                            GimpDrawable** drawables,
                            GimpProcedureConfig* config,
                            gpointer run_data
                        );
*/
static GimpValueArray*  thp_layer_magic_resize_run(
                            GimpProcedure* procedure,
                            GimpRunMode run_mode,
                            GimpImage* image,
                            GimpDrawable** drawables,
                            GimpProcedureConfig* config,
                            gpointer run_data
                        );
static GimpValueArray*  thp_image_magic_resize_run(
                            GimpProcedure* procedure,
                            GimpRunMode run_mode,
                            GimpImage* image,
                            GimpDrawable** drawables,
                            GimpProcedureConfig* config,
                            gpointer run_data
                        );
static GimpValueArray*  thp_image_magic_deflate_run(
                            GimpProcedure* procedure,
                            GimpRunMode run_mode,
                            GimpImage* image,
                            GimpDrawable** drawables,
                            GimpProcedureConfig* config,
                            gpointer run_data
                        );
static GimpValueArray*  thp_image_magic_enchant_run(
                            GimpProcedure* procedure,
                            GimpRunMode run_mode,
                            GimpImage* image,
                            GimpDrawable** drawables,
                            GimpProcedureConfig* config,
                            gpointer run_data
                        );

#endif // THP_PLUG_IN_MAIN
