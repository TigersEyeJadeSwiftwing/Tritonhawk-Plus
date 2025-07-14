#pragma once

/*
This file may come in handy if you want to build the plug-in without Codeblocks IDE.

It lists all of the stuff that Codeblocks sends to the Clang++ command line.
*/

// preprocessor defines
#undef INI_SHARED_LIB
#undef LIBDEFLATE_DLL
#undef PLUG_IN_BINARY
#undef PLUG_IN_SOURCE
#undef PLUG_IN_PROC
#undef PLUG_IN_ROLE
#undef PLUG_IN_MENU_LABEL
#undef PLUG_IN_MENU_PATH
#undef PLUG_IN_DOCUMENTATION_BLURB
#undef PLUG_IN_DOCUMENTATION_HELP_CAPTION
#undef PLUG_IN_AUTHOR
#undef PLUG_IN_COPYRIGHT
#undef PLUG_IN_DATE
#undef PLUG_IN_GUI_DIALOG_CAPTION

#define INI_SHARED_LIB
#define LIBDEFLATE_DLL
#define PLUG_IN_BINARY="(gchar*)(\"tritonhawk-plus-image-resize\")"
#define PLUG_IN_SOURCE="(gchar*)(\"tritonhawk-plus-image-resize.cpp\")"
#define PLUG_IN_PROC="(gchar*)(\"tritonhawk-plus-image-resize\")"
#define PLUG_IN_ROLE="(gchar*)(\"tritonhawk-plus-image-resize\")"
#define PLUG_IN_MENU_LABEL="(gchar*)(\"Tritonhawk Plus Multi-threaded Resize\")"
#define PLUG_IN_MENU_PATH="(gchar*)(\"<Image>/Image/Tritonhawk Plus/\")"
#define PLUG_IN_DOCUMENTATION_BLURB="(gchar*)(\"Tritonhawk Plus Multi-threaded Image Resize\")"
#define PLUG_IN_DOCUMENTATION_HELP_CAPTION="(gchar*)(\"Tritonhawk Plus Multi-threaded Image Resize\")"
#define PLUG_IN_AUTHOR="(gchar*)(\"Tiger J. Swiftwing\")"
#define PLUG_IN_COPYRIGHT="(gchar*)(\"(c) 2025 Tiger J. Swiftwing\")"
#define PLUG_IN_DATE="(gchar*)(\"July 2025 CE\")"
#define PLUG_IN_GUI_DIALOG_CAPTION="(gchar*)(\"Tritonhawk Plus Multi-threaded Image Resize\")"

// switches and options sent to clang++ for compiling
/*
-std=gnu17
-std=gnu++17
-msse4.1
-m64
-O2
-fopenmp=libomp
-Wall
*/

// stuff for the linker
/*
-std=gnu17
-std=gnu++17
-msse4.1
-m64
-O2
-fopenmp=libomp
-Wall
*/

// includes
/*
-IC:/msys64/clang64/include/gimp-3.0
-IC:/msys64/clang64/include/gtk-3.0
-IC:/msys64/clang64/include/pango-1.0
-IC:/msys64/clang64/include/harfbuzz
-IC:/msys64/clang64/include/cairo
-IC:/msys64/clang64/include/atk-1.0
-IC:/msys64/clang64/include/fribidi
-IC:/msys64/clang64/include/gegl-0.4
-IC:/msys64/clang64/include/json-glib-1.0
-IC:/msys64/clang64/include/babl-0.1
-IC:/msys64/clang64/include/freetype2
-IC:/msys64/clang64/include/pixman-1
-IC:/msys64/clang64/include/gdk-pixbuf-2.0
-IC:/msys64/clang64/include/libpng16
-IC:/msys64/clang64/include/webp
-IC:/msys64/clang64/include/gio-win32-2.0
-IC:/msys64/clang64/include/glib-2.0
-IC:/msys64/clang64/lib/glib-2.0/include
*/

// this stuff is for the linking stage
/*
-lgimpui-3.0
-lgimpwidgets-3.0
-lgimpmodule-3.0
-lgimp-3.0
-lgimpbase-3.0
-lgimpcolor-3.0
-lgimpconfig-3.0
-lgimpmath-3.0
-lgtk-3
-lgdk-3
-lz
-lgdi32
-limm32
-lshell32
-lole32
-luuid
-lwinmm
-ldwmapi
-lsetupapi
-lcfgmgr32
-lhid
-lwinspool
-lcomctl32
-lcomdlg32
-lpangocairo-1.0
-lpangowin32-1.0
-lpango-1.0
-lcairo-gobject
-latk-1.0
-lgegl-0.4
-lgegl-npd-0.4
-ljson-glib-1.0
-lbabl-0.1
-lgexiv2
-lcairo
-lharfbuzz
-lgdk_pixbuf-2.0
-lgio-2.0
-lgobject-2.0
-lgmodule-2.0
-lglib-2.0
-lintl
*/
