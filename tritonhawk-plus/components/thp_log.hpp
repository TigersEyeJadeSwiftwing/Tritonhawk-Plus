#pragma once
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

/*
#include "config_h/clang64/config.h"

#ifndef GIMP_DISABLE_COMPAR_CRUFT
#define GIMP_DISABLE_COMPAR_CRUFT
#endif

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
*/

#include "components/thp_types.hpp"

#include <chrono>
#include <string>

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

        f64                                 time_elapsed_1 = 0.0;
        f64                                 time_elapsed_2 = 0.0;
        f64                                 percent_completed_1 = 0.0;
        f64                                 percent_completed_2 = 0.0;
        f64                                 estimated_time_ms = 0.0;
        f64                                 estimated_time_seconds_f = 0.0;
        f64                                 estimated_time_minutes_f = 0.0;
        f64                                 estimated_time_hours_f = 0.0;
        u64                                 estimated_time_seconds = 0;
        u64                                 estimated_time_minutes = 0;
        u64                                 estimated_time_hours = 0;

        gboolean                            console_logging;
        gboolean                            error_console_logging;
        gboolean                            gui_logging;
        gboolean                            gui_logging_text_box;
        gboolean                            gui_logging_busy_box;
        gboolean                            gui_logging_progress_bar;

        string                              text_progress_bar;
        string                              text_data;
        string                              text_accumulated;

        TARGET_CLONES void RunLogging                     (gchar* log_message);
        TARGET_CLONES void RunLoggingConsole              (gchar* log_message);
        TARGET_CLONES void RunLoggingError                (gchar* log_message);
        TARGET_CLONES void RunLoggingGui                  (gchar* log_message);
        TARGET_CLONES void RunLoggingGuiTextBox           (gchar* log_message);
        TARGET_CLONES void RunLoggingGuiBusyBox           (gchar* log_message);
        TARGET_CLONES void RunLoggingGuiProgressBar       (gchar* log_message);
        TARGET_CLONES void RunLoggingAuto                 ();

    public:
        bool                                inside_multithread_critical;
        f64                                 progress_bar_fraction;
        f64                                 progress_increment = 1.0;
        f64                                 progress_total = 0.0;

        ThpLog();

        TARGET_CLONES f64 GetTimeMS                       ();
        TARGET_CLONES f64 GetTimeMSwithEstimate           ();
        TARGET_CLONES void SetTimerStart                  ();
        TARGET_CLONES f64 GetTimerElapsedMS               ();

        TARGET_CLONES bool Run_CheckIfShouldUpdate        ();
        TARGET_CLONES void Run1                           (gchar* log_message);
        TARGET_CLONES void Run2                           (gchar* log_message, f64 percent_completed);

        TARGET_CLONES void SetConsoleLogging              (gboolean enable=TRUE);
        TARGET_CLONES void SetErrorConsoleLogging         (gboolean enable=TRUE);
        TARGET_CLONES void SetGuiLogging                  (gboolean enable=TRUE);

        TARGET_CLONES void SetGuiDialog                   (GtkWidget* dialog_input);
        TARGET_CLONES void SetBusyBox                     (GtkWidget* busy_box_input);
        TARGET_CLONES void SetTextLabel                   (GtkWidget* gtk_text_label_input, u32 index = 0u);
        TARGET_CLONES void SetGuiTextWidget               (GtkWidget* widget_input);
        TARGET_CLONES void SetTextBuffer                  (GtkTextBuffer* text_buffer_input);
        TARGET_CLONES void SetTextBufferWidget            (GtkWidget* scrolled_text_widget_input);

        TARGET_CLONES void SetInsideCritical              (bool inside=true);

        TARGET_CLONES void Log                            (bool inside_omp_critical, gchar* log_message);
        TARGET_CLONES void Log                            (bool inside_omp_critical, string log_message);
        TARGET_CLONES void Log                            (gchar* log_message);
        TARGET_CLONES void Log                            (string log_message);

        TARGET_CLONES void LogConsole                     (bool inside_omp_critical, gchar* log_message);
        TARGET_CLONES void LogError                       (bool inside_omp_critical, gchar* log_message);
        TARGET_CLONES void LogGui                         (bool inside_omp_critical, gchar* log_message);
        TARGET_CLONES void LogGuiTextBox                  (bool inside_omp_critical, gchar* log_message);
        TARGET_CLONES void LogGuiBusyBox                  (bool inside_omp_critical, gchar* log_message);
        TARGET_CLONES void LogGuiProgressBar              (bool inside_omp_critical, gchar* log_message);

        TARGET_CLONES void AutoLog                        ();
        TARGET_CLONES void AutoLog                        (bool is_inside_omp_critical);
        TARGET_CLONES void AutoLogConsole                 ();
        TARGET_CLONES void AutoLogError                   ();
        TARGET_CLONES void AutoLogGui                     ();
        TARGET_CLONES void AutoLogGuiTextBox              ();
        TARGET_CLONES void AutoLogGuiBusyBox              ();
        TARGET_CLONES void AutoLogGuiProgressBar          ();

        TARGET_CLONES void SetStringProgressBar           (string txt);
        TARGET_CLONES void SetStringProgressBar           (gchar* txt);
        TARGET_CLONES void SetString                      (string txt);
        TARGET_CLONES void SetString                      (gchar* txt);
    };
};
