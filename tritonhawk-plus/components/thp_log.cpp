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
#include "components/thp_log.hpp"

#include <chrono>
#include <string>

using namespace std;
using clock_log = std::chrono::steady_clock;

namespace TritonhawkPlus
{
    TARGET_CLONES f64 ThpLog::GetTimeMS()
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
    TARGET_CLONES void ThpLog::SetTimerStart()
    {
        timer_procedure_start = clock_log::now();
    }
    TARGET_CLONES f64 ThpLog::GetTimerElapsedMS()
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

        console_logging = FALSE;
        error_console_logging = FALSE;
        gui_logging = TRUE;
        gui_logging_text_box = TRUE;
        gui_logging_busy_box = TRUE;
        gui_logging_progress_bar = TRUE;

        inside_multithread_critical = false;
        progress_bar_fraction = 0.0;

        time_loopbreaker = 95.0; // Should be long enough for updating text for non-time-critical stuff
        time_loopbreaker_fast = 18.0; // Should be just long enough to update the GUI text, even for a slow ~59 Hz display monitor
        timer_time_point = clock_log::now();
    }

    TARGET_CLONES bool ThpLog::Run_CheckIfShouldUpdate()
    {
        f64 time_elapsed = GetTimerElapsedMS();

        if (time_elapsed < time_loopbreaker_fast)
            return false;
        if ((time_elapsed - time_elapsed_1) < 200.0)
            return false;

        time_elapsed_1 = time_elapsed;

        return true;
    }
    TARGET_CLONES void ThpLog::Run1(gchar* log_message)
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
    TARGET_CLONES void ThpLog::Run2(gchar* log_message, f64 percent_completed)
    {
        if (!gui_gtk_textlabel_1) return;

        // f64 time_elapsed = GetTimerElapsedMS();

        time_elapsed_2 = time_elapsed_1;
        // time_elapsed_1 = time_elapsed;
        // time_elapsed_1 = GetTimerElapsedMS();
        percent_completed_2 = percent_completed_1;
        percent_completed_1 = percent_completed * 0.01;

        if (time_elapsed_2 > 0.00001)
        {
            // f64 estimate_ms_1 = (time_elapsed_1 - time_elapsed) / fmax(percent_completed_1 - percent_completed, 0.0001);
            // f64 estimate_ms_2 = (time_elapsed_2 - time_elapsed_1) / fmax(percent_completed_2 - percent_completed_1, 0.0001);
            f64 estimate_ms_1 = time_elapsed_1 / fmax(percent_completed_1, 0.00001);
            f64 estimate_ms_2 = time_elapsed_2 / fmax(percent_completed_2, 0.00001);
            f64 estimate_ms = (estimate_ms_1 + estimate_ms_2) * 0.5;
            f64 portion_left = 1.0 - ((percent_completed_1 + percent_completed_2) * 0.5);

            estimated_time_seconds_f = estimate_ms * portion_left * 0.001;
            estimated_time_minutes_f = estimated_time_seconds_f / 60.0;
            estimated_time_hours_f = estimated_time_minutes_f / 60.0;
            estimated_time_hours = u64(estimated_time_hours_f);
            estimated_time_minutes = u64(estimated_time_minutes_f) % 60;
            estimated_time_seconds = u64(estimated_time_seconds_f) % 60;
        }
        else if (time_elapsed_1 > 0.00001)
        {
            // f64 estimate_ms_1 = (time_elapsed_1 - time_elapsed) / fmax(percent_completed_1 - percent_completed, 0.0001);
            // f64 estimate_ms_2 = (time_elapsed_2 - time_elapsed_1) / fmax(percent_completed_2 - percent_completed_1, 0.0001);
            f64 estimate_ms = time_elapsed_1 / fmax(percent_completed_1, 0.00001);
            f64 portion_left = 1.0 - percent_completed_1;

            estimated_time_seconds_f = estimate_ms * portion_left * 0.001;
            estimated_time_minutes_f = estimated_time_seconds_f / 60.0;
            estimated_time_hours_f = estimated_time_minutes_f / 60.0;
            estimated_time_hours = u64(estimated_time_hours_f);
            estimated_time_minutes = u64(estimated_time_minutes_f) % 60;
            estimated_time_seconds = u64(estimated_time_seconds_f) % 60;
        }
        else
            return;

        f64 elapsed_time_seconds_f = time_elapsed_1 * 0.001;
        f64 elapsed_time_minutes_f = elapsed_time_seconds_f / 60.0;
        f64 elapsed_time_hours_f = elapsed_time_minutes_f / 60.0;
        u64 elapsed_time_hours = u64(elapsed_time_hours_f);
        u64 elapsed_time_minutes = u64(elapsed_time_minutes_f) % 60;
        u64 elapsed_time_seconds = u64(elapsed_time_seconds_f) % 60;

        f64 elapsed_percent_per_second = (percent_completed > 0.00001) ? percent_completed / fmax(elapsed_time_seconds_f, 0.001) : 0.0;

        gtk_label_set_text(
            (GtkLabel*)gui_gtk_textlabel_1,
            g_strdup_printf (_(
                "%s" "\n"
                "Estimated Total Time Left:" "\n"
                "   %I64u Hours, %I64u Minutes, %I64u Seconds" "\n"
                "Total Time Elapsed:" "\n"
                "   %I64u Hours, %I64u Minutes, %I64u Seconds" "\n"
                "\n"
                "Progress / Second: %%%3.5lf"
            ),
                log_message,
                (estimated_time_hours), (estimated_time_minutes), (estimated_time_seconds),
                (elapsed_time_hours), (elapsed_time_minutes), (elapsed_time_seconds),
                elapsed_percent_per_second
            ));

        if ( !gtk_events_pending() ) return;

        f64 elapsed_time_loopbreaking = GetTimerElapsedMS() + time_loopbreaker_fast;
        while ( gtk_events_pending() )
        {
            gtk_main_iteration();

            if (GetTimerElapsedMS() >= elapsed_time_loopbreaking) break;
        }
    }
    TARGET_CLONES void ThpLog::RunLogging(gchar* log_message)
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

        /*
        GetTimeMS();
        f64 elapsed_time = 0.0;
        while ( gtk_events_pending() )
        {
            elapsed_time += GetTimeMS();
            if (elapsed_time >= time_loopbreaker)
                break;

            gtk_main_iteration();
        }
        */

        if ( !gtk_events_pending() ) return;

        f64 elapsed_time_loopbreaking = GetTimerElapsedMS() + time_loopbreaker_fast;
        while ( gtk_events_pending() )
        {
            gtk_main_iteration();

            if (GetTimerElapsedMS() >= elapsed_time_loopbreaking) break;
        }
    }
    TARGET_CLONES void ThpLog::RunLoggingConsole(gchar* log_message)
    {
        if (console_logging)
        {
            GimpMessageHandlerType old_message_handler = gimp_message_get_handler();
            gimp_message_set_handler(GIMP_CONSOLE);
            gimp_message( g_strdup_printf (_("%s"), log_message) );
            gimp_message_set_handler(old_message_handler);
        }
    }
    TARGET_CLONES void ThpLog::RunLoggingError(gchar* log_message)
    {
        if (error_console_logging)
        {
            GimpMessageHandlerType old_message_handler = gimp_message_get_handler();
            gimp_message_set_handler(GIMP_ERROR_CONSOLE);
            gimp_message( g_strdup_printf (_("%s"), log_message) );
            gimp_message_set_handler(old_message_handler);
        }
    }
    TARGET_CLONES void ThpLog::RunLoggingGui(gchar* log_message)
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
    TARGET_CLONES void ThpLog::RunLoggingGuiTextBox(gchar* log_message)
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
    TARGET_CLONES void ThpLog::RunLoggingGuiBusyBox(gchar* log_message)
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
    TARGET_CLONES void ThpLog::RunLoggingGuiProgressBar(gchar* log_message)
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
    TARGET_CLONES void ThpLog::RunLoggingAuto()
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

    TARGET_CLONES void ThpLog::SetConsoleLogging(gboolean enable)
    {
        console_logging = enable;
    }
    TARGET_CLONES void ThpLog::SetErrorConsoleLogging(gboolean enable)
    {
        error_console_logging = enable;
    }
    TARGET_CLONES void ThpLog::SetGuiLogging(gboolean enable)
    {
        gui_logging = enable;
    }
    TARGET_CLONES void ThpLog::SetGuiDialog(GtkWidget* dialog_input)
    {
        gui_dialog = dialog_input;
    }
    TARGET_CLONES void ThpLog::SetBusyBox(GtkWidget* busy_box_input)
    {
        gui_busy_box = busy_box_input;
    }
    TARGET_CLONES void ThpLog::SetTextLabel(GtkWidget* gtk_text_label_input, u32 index)
    {
        if (index == 0u)
            gui_gtk_textlabel_0 = gtk_text_label_input;
        else if (index == 1u)
            gui_gtk_textlabel_1 = gtk_text_label_input;
    }
    TARGET_CLONES void ThpLog::SetGuiTextWidget(GtkWidget* widget_input)
    {
        gui_text_widget = widget_input;
    }
    TARGET_CLONES void ThpLog::SetTextBuffer(GtkTextBuffer *text_buffer_input)
    {
        gui_text_buffer = text_buffer_input;
    }
    TARGET_CLONES void ThpLog::SetTextBufferWidget(GtkWidget *scrolled_text_widget_input)
    {
        gui_scrolled_text_widget = scrolled_text_widget_input;
    }

    TARGET_CLONES void ThpLog::SetInsideCritical(bool inside)
    {
        inside_multithread_critical = inside;
    }

    TARGET_CLONES void ThpLog::Log(bool inside_omp_critical, gchar* log_message)
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
    TARGET_CLONES void ThpLog::Log(bool inside_omp_critical, string log_message)
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
    TARGET_CLONES void ThpLog::Log(gchar* log_message)
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
    TARGET_CLONES void ThpLog::Log(string log_message)
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

    TARGET_CLONES void ThpLog::LogConsole(bool inside_omp_critical, gchar* log_message)
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
    TARGET_CLONES void ThpLog::LogError(bool inside_omp_critical, gchar* log_message)
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
    TARGET_CLONES void ThpLog::LogGui(bool inside_omp_critical, gchar* log_message)
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
    TARGET_CLONES void ThpLog::LogGuiTextBox(bool inside_omp_critical, gchar* log_message)
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
    TARGET_CLONES void ThpLog::LogGuiBusyBox(bool inside_omp_critical, gchar* log_message)
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
    TARGET_CLONES void ThpLog::LogGuiProgressBar(bool inside_omp_critical, gchar* log_message)
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

    TARGET_CLONES void ThpLog::AutoLog()
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
    TARGET_CLONES void ThpLog::AutoLog(bool is_inside_omp_critical)
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
    TARGET_CLONES void ThpLog::AutoLogConsole()
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
    TARGET_CLONES void ThpLog::AutoLogError()
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
    TARGET_CLONES void ThpLog::AutoLogGui()
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
    TARGET_CLONES void ThpLog::AutoLogGuiTextBox()
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
    TARGET_CLONES void ThpLog::AutoLogGuiBusyBox()
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
    TARGET_CLONES void ThpLog::AutoLogGuiProgressBar()
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

    TARGET_CLONES void ThpLog::SetStringProgressBar(string txt)
    {
        text_progress_bar = txt;
    }
    TARGET_CLONES void ThpLog::SetStringProgressBar(gchar* txt)
    {
        text_progress_bar = string(txt);
        g_free(txt);
    }
    TARGET_CLONES void ThpLog::SetString(string txt)
    {
        text_data = txt;
        if (gui_logging_text_box && gui_text_buffer)
            text_accumulated.append(txt);
    }
    TARGET_CLONES void ThpLog::SetString(gchar* txt)
    {
        text_data = string(txt);
        if (gui_logging_text_box && gui_text_buffer)
            text_accumulated.append(string(txt));
        g_free(txt);
    }
};
