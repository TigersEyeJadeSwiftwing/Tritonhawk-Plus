#pragma once

#include "config_h/clang64/config.h"

#ifndef GIMP_DISABLE_COMPAR_CRUFT
#define GIMP_DISABLE_COMPAR_CRUFT
#endif

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "components/thp_types.h"

#include <chrono>
#include <string>

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

using namespace std;

namespace TritonhawkPlus
{
    class ThpLog
    {
    private:
        GtkWidget*                          gui_dialog;
        GtkWidget*                          gui_busy_box;
        GtkWidget*                          gui_gtk_textlabel_0;
        GtkWidget*                          gui_gtk_textlabel_1;
        GtkWidget*                          gui_progress_bar;
        GtkWidget*                          gui_text_widget;
        GtkTextBuffer*                      gui_text_buffer;
        GtkWidget*                          gui_scrolled_text_widget;

        std::chrono::time_point<std::chrono::steady_clock> timer_time_point;
        std::chrono::time_point<std::chrono::steady_clock> timer_procedure_start;
        f64                                 time_loopbreaker;
        f64                                 time_loopbreaker_fast;

        gboolean                            console_logging;
        gboolean                            error_console_logging;
        gboolean                            gui_logging;
        gboolean                            gui_logging_text_box;
        gboolean                            gui_logging_busy_box;
        gboolean                            gui_logging_progress_bar;

        string                              text_progress_bar;
        string                              text_data;
        string                              text_accumulated;

        void RunLogging                     (gchar* log_message);
        void RunLoggingConsole              (gchar* log_message);
        void RunLoggingError                (gchar* log_message);
        void RunLoggingGui                  (gchar* log_message);
        void RunLoggingGuiTextBox           (gchar* log_message);
        void RunLoggingGuiBusyBox           (gchar* log_message);
        void RunLoggingGuiProgressBar       (gchar* log_message);
        void RunLoggingAuto                 ();

    public:
        bool                                inside_multithread_critical;
        f64                                 progress_bar_fraction;

        ThpLog();

        f64 GetTimeMS                       ();
        void SetTimerStart                  ();
        f64 GetTimerElapsedMS               ();

        void Run1                           (gchar* log_message);

        void SetConsoleLogging              (gboolean enable=TRUE);
        void SetErrorConsoleLogging         (gboolean enable=TRUE);
        void SetGuiLogging                  (gboolean enable=TRUE);

        void SetGuiDialog                   (GtkWidget* dialog_input);
        void SetBusyBox                     (GtkWidget* busy_box_input);
        void SetTextLabel                   (GtkWidget* gtk_text_label_input, u32 index = 0u);
        void SetGuiTextWidget               (GtkWidget* widget_input);
        void SetTextBuffer                  (GtkTextBuffer* text_buffer_input);
        void SetTextBufferWidget            (GtkWidget* scrolled_text_widget_input);

        void SetInsideCritical              (bool inside=true);

        void Log                            (bool inside_omp_critical, gchar* log_message);
        void Log                            (bool inside_omp_critical, string log_message);
        void Log                            (gchar* log_message);
        void Log                            (string log_message);

        void LogConsole                     (bool inside_omp_critical, gchar* log_message);
        void LogError                       (bool inside_omp_critical, gchar* log_message);
        void LogGui                         (bool inside_omp_critical, gchar* log_message);
        void LogGuiTextBox                  (bool inside_omp_critical, gchar* log_message);
        void LogGuiBusyBox                  (bool inside_omp_critical, gchar* log_message);
        void LogGuiProgressBar              (bool inside_omp_critical, gchar* log_message);

        void AutoLog                        ();
        void AutoLog                        (bool is_inside_omp_critical);
        void AutoLogConsole                 ();
        void AutoLogError                   ();
        void AutoLogGui                     ();
        void AutoLogGuiTextBox              ();
        void AutoLogGuiBusyBox              ();
        void AutoLogGuiProgressBar          ();

        void SetStringProgressBar           (string txt);
        void SetStringProgressBar           (gchar* txt);
        void SetString                      (string txt);
        void SetString                      (gchar* txt);
    };
};
