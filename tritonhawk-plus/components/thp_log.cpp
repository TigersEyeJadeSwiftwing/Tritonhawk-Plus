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

#include "components/thp_types.h"
#include "components/thp_log.h"

#include <chrono>
#include <string>

using namespace std;
using clock_log = std::chrono::steady_clock;

namespace TritonhawkPlus
{
    f64 ThpLog::GetTimeMS()
    {
        /*
        std::chrono::time_point<std::chrono::steady_clock> current_time = clock_log::now();
        auto diff = current_time - timer_time_point;
        timer_time_point = current_time;
        f64 time_ms = std::chrono::duration<double, std::milli>(diff).count();
        return time_ms;
        */

        std::chrono::time_point<std::chrono::steady_clock> current_time = clock_log::now();
        f64 time_ms = std::chrono::duration<double, std::milli>(current_time - timer_time_point).count();
        timer_time_point = current_time;
        return time_ms;
    }
    void ThpLog::SetTimerStart()
    {
        timer_procedure_start = clock_log::now();
    }
    f64 ThpLog::GetTimerElapsedMS()
    {
        std::chrono::time_point<std::chrono::steady_clock> current_time = clock_log::now();
        // auto diff = current_time - timer_procedure_start;
        return std::chrono::duration<double, std::milli>(current_time - timer_procedure_start).count();
    }

    ThpLog::ThpLog()
    {
        gui_dialog = NULL;
        gui_busy_box = NULL;
        gui_progress_bar = NULL;
        gui_text_widget = NULL;
        gui_text_buffer = NULL;
        gui_scrolled_text_widget = NULL;
        gui_gtk_textlabel_0 = NULL;
        gui_gtk_textlabel_1 = NULL;

        console_logging = TRUE;
        error_console_logging = FALSE;
        gui_logging = TRUE;
        gui_logging_text_box = TRUE;
        gui_logging_busy_box = TRUE;
        gui_logging_progress_bar = TRUE;

        inside_multithread_critical = false;
        progress_bar_fraction = 0.0;

        time_loopbreaker = 95.0; // Should be long enough for updating text for non-time-critical stuff
        time_loopbreaker_fast = 17.0; // Should be just long enough to update the GUI text, even for a slow ~60 Hz display monitor
        timer_time_point = clock_log::now();
    }

    void ThpLog::Run1(gchar* log_message)
    {
        if (!gui_gtk_textlabel_1) return;

        gtk_label_set_text((GtkLabel*)gui_gtk_textlabel_1, log_message);

        f64 elapsed_time = 0.0;
        GetTimeMS();
        while ( gtk_events_pending() )
        {
            gtk_main_iteration();

            elapsed_time += GetTimeMS();
            if (elapsed_time >= time_loopbreaker_fast) break;
        }
    }
    void ThpLog::RunLogging(gchar* log_message)
    {
        if (console_logging)
        {
            GimpMessageHandlerType old_message_handler = gimp_message_get_handler();
            gimp_message_set_handler(GIMP_CONSOLE);
            gimp_message( g_strdup_printf (_("%s"), log_message) );
            gimp_message_set_handler(old_message_handler);
        }
        if (error_console_logging)
        {
            GimpMessageHandlerType old_message_handler = gimp_message_get_handler();
            gimp_message_set_handler(GIMP_ERROR_CONSOLE);
            gimp_message( g_strdup_printf (_("%s"), log_message) );
            gimp_message_set_handler(old_message_handler);
        }
        if (gui_logging)
        {
            if (gui_logging_text_box && gui_text_buffer)
            {
                gint l_size = (gint)std::string(log_message).size();
                gtk_text_buffer_insert_at_cursor(gui_text_buffer, log_message, l_size);
            }
            if (gui_logging_busy_box && gui_busy_box)
            {
                gimp_busy_box_set_message((GimpBusyBox*)gui_busy_box, log_message);
            }
            if (gui_gtk_textlabel_0)
            {
                gtk_label_set_text((GtkLabel*)gui_gtk_textlabel_0, log_message);
            }
            if (gui_logging_progress_bar && gui_progress_bar)
            {
                gtk_progress_bar_set_fraction ((GtkProgressBar*)gui_progress_bar, progress_bar_fraction);
                gtk_progress_bar_set_text ((GtkProgressBar*)gui_progress_bar, log_message);
            }
        }

        GetTimeMS();
        f64 elapsed_time = 0.0;
        while ( gtk_events_pending() )
        {
            elapsed_time += GetTimeMS();
            if (elapsed_time >= time_loopbreaker)
                break;

            gtk_main_iteration();
        }
    }
    void ThpLog::RunLoggingConsole(gchar* log_message)
    {
        if (console_logging)
        {
            GimpMessageHandlerType old_message_handler = gimp_message_get_handler();
            gimp_message_set_handler(GIMP_CONSOLE);
            gimp_message( g_strdup_printf (_("%s"), log_message) );
            gimp_message_set_handler(old_message_handler);
        }
    }
    void ThpLog::RunLoggingError(gchar* log_message)
    {
        if (error_console_logging)
        {
            GimpMessageHandlerType old_message_handler = gimp_message_get_handler();
            gimp_message_set_handler(GIMP_ERROR_CONSOLE);
            gimp_message( g_strdup_printf (_("%s"), log_message) );
            gimp_message_set_handler(old_message_handler);
        }
    }
    void ThpLog::RunLoggingGui(gchar* log_message)
    {
        if (gui_logging)
        {
            if (gui_logging_text_box && gui_text_buffer)
            {
                gint l_size = (gint)std::string(log_message).size();
                gtk_text_buffer_insert_at_cursor(gui_text_buffer, log_message, l_size);
            }
            if (gui_logging_busy_box && gui_busy_box)
            {
                gimp_busy_box_set_message((GimpBusyBox*)gui_busy_box, log_message);
            }
            if (gui_gtk_textlabel_0)
            {
                gtk_label_set_text((GtkLabel*)gui_gtk_textlabel_0, log_message);
            }
            if (gui_logging_progress_bar && gui_progress_bar)
            {
                gtk_progress_bar_set_fraction ((GtkProgressBar*)gui_progress_bar, progress_bar_fraction);
                gtk_progress_bar_set_text ((GtkProgressBar*)gui_progress_bar, log_message);
            }
        }

        GetTimeMS();
        f64 elapsed_time = 0.0;
        while ( gtk_events_pending() )
        {
            elapsed_time += GetTimeMS();
            if (elapsed_time >= time_loopbreaker)
                break;

            gtk_main_iteration();
        }
    }
    void ThpLog::RunLoggingGuiTextBox(gchar* log_message)
    {
        if (gui_logging)
        {
            if (gui_logging_text_box && gui_text_buffer)
            {
                gint l_size = (gint)std::string(log_message).size();
                gtk_text_buffer_insert_at_cursor(gui_text_buffer, log_message, l_size);
            }
        }

        GetTimeMS();
        f64 elapsed_time = 0.0;
        while ( gtk_events_pending() )
        {
            elapsed_time += GetTimeMS();
            if (elapsed_time >= time_loopbreaker)
                break;

            gtk_main_iteration();
        }
    }
    void ThpLog::RunLoggingGuiBusyBox(gchar* log_message)
    {
        if (gui_logging)
        {
            if (gui_logging_busy_box && gui_busy_box)
            {
                gimp_busy_box_set_message((GimpBusyBox*)gui_busy_box, log_message);
            }
            if (gui_gtk_textlabel_0)
            {
                gtk_label_set_text((GtkLabel*)gui_gtk_textlabel_0, log_message);
            }
        }

        GetTimeMS();
        f64 elapsed_time = 0.0;
        while ( gtk_events_pending() )
        {
            elapsed_time += GetTimeMS();
            if (elapsed_time >= time_loopbreaker)
                break;

            gtk_main_iteration();
        }
    }
    void ThpLog::RunLoggingGuiProgressBar(gchar* log_message)
    {
        if (gui_logging)
        {
            if (gui_logging_progress_bar && gui_progress_bar)
            {
                gtk_progress_bar_set_fraction ((GtkProgressBar*)gui_progress_bar, progress_bar_fraction);
                gtk_progress_bar_set_text ((GtkProgressBar*)gui_progress_bar, log_message);
            }
        }

        GetTimeMS();
        f64 elapsed_time = 0.0;
        while ( gtk_events_pending() )
        {
            elapsed_time += GetTimeMS();
            if (elapsed_time >= time_loopbreaker)
                break;

            gtk_main_iteration();
        }
    }
    void ThpLog::RunLoggingAuto()
    {
        gchar* log_txt = g_strdup_printf (_("%s"), text_data.c_str());
        gchar* log_long = g_strdup_printf (_("%s"), text_accumulated.c_str());
        gchar* log_progress = g_strdup_printf (_("%s"), text_progress_bar.c_str());

        if (console_logging)
        {
            GimpMessageHandlerType old_message_handler = gimp_message_get_handler();
            gimp_message_set_handler(GIMP_CONSOLE);
            gimp_message( log_txt );
            gimp_message_set_handler(old_message_handler);
        }
        if (error_console_logging)
        {
            GimpMessageHandlerType old_message_handler = gimp_message_get_handler();
            gimp_message_set_handler(GIMP_ERROR_CONSOLE);
            gimp_message( log_txt );
            gimp_message_set_handler(old_message_handler);
        }
        if (gui_logging)
        {
            if (gui_logging_text_box && gui_text_buffer)
            {
                gint l_size = (gint)std::string(log_long).size();
                gtk_text_buffer_set_text (gui_text_buffer, log_long, l_size);
            }
            if (gui_logging_busy_box && gui_busy_box)
            {
                gimp_busy_box_set_message((GimpBusyBox*)gui_busy_box, log_txt);
            }
            if (gui_gtk_textlabel_0)
            {
                gtk_label_set_text((GtkLabel*)gui_gtk_textlabel_0, log_txt);
            }
            if (gui_logging_progress_bar && gui_progress_bar)
            {
                gtk_progress_bar_set_fraction ((GtkProgressBar*)gui_progress_bar, progress_bar_fraction);
                gtk_progress_bar_set_text ((GtkProgressBar*)gui_progress_bar, log_progress);
            }
        }

        GetTimeMS();
        f64 elapsed_time = 0.0;
        while ( gtk_events_pending() )
        {
            elapsed_time += GetTimeMS();
            if (elapsed_time >= time_loopbreaker)
                break;

            gtk_main_iteration();
        }

        g_free(log_txt);
        g_free(log_long);
        g_free(log_progress);
    }

    void ThpLog::SetConsoleLogging(gboolean enable)
    {
        console_logging = enable;
    }
    void ThpLog::SetErrorConsoleLogging(gboolean enable)
    {
        error_console_logging = enable;
    }
    void ThpLog::SetGuiLogging(gboolean enable)
    {
        gui_logging = enable;
    }
    void ThpLog::SetGuiDialog(GtkWidget* dialog_input)
    {
        gui_dialog = dialog_input;
    }
    void ThpLog::SetBusyBox(GtkWidget* busy_box_input)
    {
        gui_busy_box = busy_box_input;
    }
    void ThpLog::SetTextLabel(GtkWidget* gtk_text_label_input, u32 index)
    {
        if (index == 0u)
            gui_gtk_textlabel_0 = gtk_text_label_input;
        else if (index == 1u)
            gui_gtk_textlabel_1 = gtk_text_label_input;
    }
    void ThpLog::SetGuiTextWidget(GtkWidget* widget_input)
    {
        gui_text_widget = widget_input;
    }
    void ThpLog::SetTextBuffer(GtkTextBuffer *text_buffer_input)
    {
        gui_text_buffer = text_buffer_input;
    }
    void ThpLog::SetTextBufferWidget(GtkWidget *scrolled_text_widget_input)
    {
        gui_scrolled_text_widget = scrolled_text_widget_input;
    }

    void ThpLog::SetInsideCritical(bool inside)
    {
        inside_multithread_critical = inside;
    }

    void ThpLog::Log(bool inside_omp_critical, gchar* log_message)
    {
        if (!inside_omp_critical)
        {
            #pragma omp critical
            RunLogging(log_message);
        }
        else
        {
            RunLogging(log_message);
        }

        g_free(log_message);
    }
    void ThpLog::Log(bool inside_omp_critical, string log_message)
    {
        gchar* log_m = g_strdup_printf( _("%s"), log_message.c_str());

        if (!inside_omp_critical)
        {
            #pragma omp critical
            RunLogging(log_m);
        }
        else
        {
            RunLogging(log_m);
        }

        g_free(log_m);
    }
    void ThpLog::Log(gchar* log_message)
    {
        if (!inside_multithread_critical)
        {
            #pragma omp critical
            RunLogging(log_message);
        }
        else
        {
            RunLogging(log_message);
        }

        g_free(log_message);
    }
    void ThpLog::Log(string log_message)
    {
        gchar* log_m = g_strdup_printf( _("%s"), log_message.c_str());

        if (!inside_multithread_critical)
        {
            #pragma omp critical
            RunLogging(log_m);
        }
        else
        {
            RunLogging(log_m);
        }

        g_free(log_m);
    }

    void ThpLog::LogConsole(bool inside_omp_critical, gchar* log_message)
    {
        if (!inside_omp_critical)
        {
            #pragma omp critical
            RunLoggingConsole(log_message);
        }
        else
        {
            RunLoggingConsole(log_message);
        }
    }
    void ThpLog::LogError(bool inside_omp_critical, gchar* log_message)
    {
        if (!inside_omp_critical)
        {
            #pragma omp critical
            RunLoggingError(log_message);
        }
        else
        {
            RunLoggingError(log_message);
        }
    }
    void ThpLog::LogGui(bool inside_omp_critical, gchar* log_message)
    {
        if (!inside_omp_critical)
        {
            #pragma omp critical
            RunLoggingGui(log_message);
        }
        else
        {
            RunLoggingGui(log_message);
        }
    }
    void ThpLog::LogGuiTextBox(bool inside_omp_critical, gchar* log_message)
    {
        if (!inside_omp_critical)
        {
            #pragma omp critical
            RunLoggingGuiTextBox(log_message);
        }
        else
        {
            RunLoggingGuiTextBox(log_message);
        }
    }
    void ThpLog::LogGuiBusyBox(bool inside_omp_critical, gchar* log_message)
    {
        if (!inside_omp_critical)
        {
            #pragma omp critical
            RunLoggingGuiBusyBox(log_message);
        }
        else
        {
            RunLoggingGuiBusyBox(log_message);
        }
    }
    void ThpLog::LogGuiProgressBar(bool inside_omp_critical, gchar* log_message)
    {
        if (!inside_omp_critical)
        {
            #pragma omp critical
            RunLoggingGuiProgressBar(log_message);
        }
        else
        {
            RunLoggingGuiProgressBar(log_message);
        }
    }

    void ThpLog::AutoLog()
    {
        if (!inside_multithread_critical)
        {
            #pragma omp critical
            RunLoggingAuto();
        }
        else
        {
            RunLoggingAuto();
        }
    }
    void ThpLog::AutoLog(bool is_inside_omp_critical)
    {
        if (!is_inside_omp_critical)
        {
            #pragma omp critical
            RunLoggingAuto();
        }
        else
        {
            RunLoggingAuto();
        }
    }
    void ThpLog::AutoLogConsole()
    {
        gchar* log_message = g_strdup_printf (_("%s"), text_data.c_str());

        if (!inside_multithread_critical)
        {
            #pragma omp critical
            RunLoggingConsole(log_message);
        }
        else
        {
            RunLoggingConsole(log_message);
        }

        g_free(log_message);
    }
    void ThpLog::AutoLogError()
    {
        gchar* log_message = g_strdup_printf (_("%s"), text_data.c_str());

        if (!inside_multithread_critical)
        {
            #pragma omp critical
            RunLoggingError(log_message);
        }
        else
        {
            RunLoggingError(log_message);
        }

        g_free(log_message);
    }
    void ThpLog::AutoLogGui()
    {
        gchar* log_message = g_strdup_printf (_("%s"), text_data.c_str());

        if (!inside_multithread_critical)
        {
            #pragma omp critical
            RunLoggingGui(log_message);
        }
        else
        {
            RunLoggingGui(log_message);
        }

        g_free(log_message);
    }
    void ThpLog::AutoLogGuiTextBox()
    {
        gchar* log_message = g_strdup_printf (_("%s"), text_accumulated.c_str());

        if (!inside_multithread_critical)
        {
            #pragma omp critical
            RunLoggingGuiTextBox(log_message);
        }
        else
        {
            RunLoggingGuiTextBox(log_message);
        }

        g_free(log_message);
    }
    void ThpLog::AutoLogGuiBusyBox()
    {
        gchar* log_message = g_strdup_printf (_("%s"), text_data.c_str());

        if (!inside_multithread_critical)
        {
            #pragma omp critical
            RunLoggingGuiBusyBox(log_message);
        }
        else
        {
            RunLoggingGuiBusyBox(log_message);
        }

        g_free(log_message);
    }
    void ThpLog::AutoLogGuiProgressBar()
    {
        gchar* log_message = g_strdup_printf (_("%s"), text_progress_bar.c_str());

        if (!inside_multithread_critical)
        {
            #pragma omp critical
            RunLoggingGuiProgressBar(log_message);
        }
        else
        {
            RunLoggingGuiProgressBar(log_message);
        }

        g_free(log_message);
    }

    void ThpLog::SetStringProgressBar(string txt)
    {
        text_progress_bar = txt;
    }
    void ThpLog::SetStringProgressBar(gchar* txt)
    {
        text_progress_bar = string(txt);
        g_free(txt);
    }
    void ThpLog::SetString(string txt)
    {
        text_data = txt;
        if (gui_logging_text_box && gui_text_buffer)
            text_accumulated.append(txt);
    }
    void ThpLog::SetString(gchar* txt)
    {
        text_data = string(txt);
        if (gui_logging_text_box && gui_text_buffer)
            text_accumulated.append(string(txt));
        g_free(txt);
    }
};
