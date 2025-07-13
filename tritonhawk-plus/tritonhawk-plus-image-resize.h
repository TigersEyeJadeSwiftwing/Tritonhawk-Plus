#pragma once

#include "config_h/clang64/config.h"

#ifndef GIMP_DISABLE_COMPAR_CRUFT
#define GIMP_DISABLE_COMPAR_CRUFT
#endif

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include <libgimp/stdplugins-intl.h>

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
