#pragma once
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

#include "config_h/clang64/config.h"

#ifndef GIMP_DISABLE_COMPAR_CRUFT
#define GIMP_DISABLE_COMPAR_CRUFT
#endif

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include <omp.h>
#include "components/thp_types.h"
#include <string>

using namespace std;

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
static GimpValueArray*  thpimageresize_run(
                            GimpProcedure* procedure,
                            GimpRunMode run_mode,
                            GimpImage* image,
                            GimpDrawable** drawables,
                            GimpProcedureConfig* config,
                            gpointer run_data
                        );
