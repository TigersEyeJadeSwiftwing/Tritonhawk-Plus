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

#include "components/thp_types.h"
#include "components/thp_parameters.h"
#include "components/thp_log.h"

using namespace TritonhawkPlus;
using namespace std;

namespace TritonhawkPlus
{
    class ComboSizeWidget
    {
    public:
        ComboSizeWidget(GtkWidget* Program_Dialog_in, ThpParams* Params_in=NULL, ThpLog* Log_in=NULL);
        ~ComboSizeWidget();

        void SetSizeX(gint val);
        void SetSizeY(gint val);
        void SetScaleX(gdouble val);
        void SetScaleY(gdouble val);
        void SetChainButton(gboolean val);
        void ToggleChainButton();
        void SetOriginalSize(gint val_x, gint val_y);
        void SetScale_SampleGridScaleX(gdouble val);
        void SetScale_SampleGridScaleY(gdouble val);
        void SetChainButton_SampleGridScale(gboolean val);
        void SetSampleGridWeighting(gdouble val);
        void SetSampleCountAdj(gdouble val);
        void SetSampleInterpolationX(gdouble val);
        void SetSampleInterpolationY(gdouble val);
        void SetChainButton_SampleInterpolation(gboolean val);
        void SetKilopixels(gint kpx);
        void SetButtonClicked(u16* button_name);
        s8 GetChoicesDoneResult();

    private:
        void ShowWidget(GtkWidget* gw);
        void UpdateGUI();
        void SyncDataToParameters();
        void SyncDataFromParameters();
        f128 SetRatio(gint vx, gint vy);
        f128 SetRatio(f64 vx, f64 vy);
        f128 SetRatio(f128 vx, f128 vy);
        string SetInterpolationString(f64 v);

        GtkWidget* Program_Dialog = NULL;
        ThpParams* Params = NULL;
        ThpLog*    Log = NULL;

        // Master GUI Box Area
        GtkWidget* Gui_Box_Master = NULL;
            // Row 0, image size/scale controls
            GtkWidget* Gui_Box_H_Row_0 = NULL;
                GtkWidget* Gui_Box_V_Size_Original = NULL;
                    GtkWidget* Gui_Text_Size_Original_Label = NULL;
                    GtkWidget* Gui_Text_Size_Original_X = NULL;
                    GtkWidget* Gui_Text_Size_Original_Y = NULL;
                GtkWidget* Gui_Box_V_Size_Pixels = NULL;
                    GtkWidget* Gui_Text_Size_New_Pixels_Label = NULL;
                    GtkWidget* Gui_SpinButton_Size_X_Pixels = NULL;
                    GtkWidget* Gui_SpinButton_Size_Y_Pixels = NULL;
                GtkWidget* Gui_Box_V_Size_Scale = NULL;
                    GtkWidget* Gui_Text_Size_New_Scale_Label = NULL;
                    GtkWidget* Gui_SpinButton_Size_X_Scale = NULL;
                    GtkWidget* Gui_SpinButton_Size_Y_Scale = NULL;
                GtkWidget* Gui_Box_V_Chainbutton = NULL;
                    GtkWidget* Gui_Box_V_Spacer = NULL;
                    GtkWidget* Gui_ChainButton = NULL;
            // Row 1, image size/scale quickset buttons
            GtkWidget* Gui_Box_H_Row_1 = NULL;
                    GtkWidget* Gui_Button_Size_Reset = NULL;
                    GtkWidget* Gui_Button_flip_xy = NULL;
                GtkWidget* Gui_Box_H_Row_1_Spacer_1 = NULL;
                    GtkWidget* Gui_Button_m2 = NULL;
                    GtkWidget* Gui_Button_m4 = NULL;
                    GtkWidget* Gui_Button_m5 = NULL;
                    GtkWidget* Gui_Button_m10 = NULL;
                GtkWidget* Gui_Box_H_Row_1_Spacer_2 = NULL;
                    GtkWidget* Gui_Button_d2 = NULL;
                    GtkWidget* Gui_Button_d4 = NULL;
                    GtkWidget* Gui_Button_d5 = NULL;
                    GtkWidget* Gui_Button_d10 = NULL;
            // Row 2, sample grid scale controls
            GtkWidget* Gui_Box_H_Row_2 = NULL;
                GtkWidget* Gui_Box_V_Sample_Grid_Scale_Labels = NULL;
                    GtkWidget* Gui_SpinButton_Sample_Grid_Scale_X_Label = NULL;
                    GtkWidget* Gui_SpinButton_Sample_Grid_Scale_Y_Label = NULL;
                GtkWidget* Gui_Box_V_Sample_Grid_Scale_SpinButtons = NULL;
                    GtkWidget* Gui_SpinButton_Sample_Grid_Scale_X = NULL;
                    GtkWidget* Gui_SpinButton_Sample_Grid_Scale_Y = NULL;
                    GtkWidget* Gui_ChainButton_Sample_Grid_Scale = NULL;
                GtkWidget* Gui_Box_V_Sample_Grid_Scale_QuickSetButtons = NULL;
                    GtkWidget* Gui_Box_H_Sample_Grid_Scale_QuickSet_0 = NULL;
                        GtkWidget* Gui_Button_SampleGridScale_Reset = NULL;
                        GtkWidget* Gui_Button_SampleGridScale_125 = NULL;
                        GtkWidget* Gui_Button_SampleGridScale_150 = NULL;
                        GtkWidget* Gui_Button_SampleGridScale_200 = NULL;
                    GtkWidget* Gui_Box_H_Sample_Grid_Scale_QuickSet_1 = NULL;
                        GtkWidget* Gui_Button_SampleGridScale_250 = NULL;
                        GtkWidget* Gui_Button_SampleGridScale_325 = NULL;
                        GtkWidget* Gui_Button_SampleGridScale_500 = NULL;
                        GtkWidget* Gui_Button_SampleGridScale_1200 = NULL;
            // Row 3, sample grid shape controls
            GtkWidget* Gui_Box_H_Row_3 = NULL;
                GtkWidget* Gui_Sample_Grid_Shape_Label = NULL;
                GtkWidget* Gui_Button_Grid_Shape_Auto = NULL;
                GtkWidget* Gui_Button_Grid_Shape_Square = NULL;
                GtkWidget* Gui_Button_Grid_Shape_Circle = NULL;
            // Row 4, sample weighting and count controls
            GtkWidget* Gui_Box_H_Row_4 = NULL;
                GtkWidget* Gui_Sample_Weighting_Label = NULL;
                    GtkWidget* Gui_SpinButton_Sample_Weighting = NULL;
                GtkWidget* Gui_Sample_Count_Label = NULL;
                    GtkWidget* Gui_SpinButton_Sample_Count = NULL;
            // Row 5, sample interpolation controls
            GtkWidget* Gui_Box_H_Row_5 = NULL;
                GtkWidget* Gui_Box_V_Sample_Interpolation_Labels = NULL;
                    GtkWidget* Gui_Sample_Interpolation_X_Label = NULL;
                    GtkWidget* Gui_Sample_Interpolation_Y_Label = NULL;
                GtkWidget* Gui_Box_V_Sample_Interpolation_SpinButtons = NULL;
                    GtkWidget* Gui_Sample_Interpolation_X_SpinButton = NULL;
                    GtkWidget* Gui_Sample_Interpolation_Y_SpinButton = NULL;
                    GtkWidget* Gui_Sample_Interpolation_ChainButton = NULL;
                GtkWidget* Gui_Box_V_Sample_Interpolation_Quickset = NULL;
                    GtkWidget* Gui_Box_H_Sample_Interpolation_Quickset_0 = NULL;
                        GtkWidget* Gui_Button_Sample_Lerp_Quickset_Nearest = NULL;
                        GtkWidget* Gui_Button_Sample_Lerp_Quickset_Linear = NULL;
                        GtkWidget* Gui_Button_Sample_Lerp_Quickset_Quadratic = NULL;
                    GtkWidget* Gui_Box_H_Sample_Interpolation_Quickset_1 = NULL;
                        GtkWidget* Gui_Button_Sample_Lerp_Quickset_Cubic = NULL;
                        GtkWidget* Gui_Button_Sample_Lerp_Quickset_Quartic = NULL;
                        GtkWidget* Gui_Button_Sample_Lerp_Quickset_Quintic = NULL;
            // Row 6, border wrapping controls
            GtkWidget* Gui_Box_H_Row_6 = NULL;
                GtkWidget* Gui_Button_Seamless_X = NULL;
                GtkWidget* Gui_Button_Seamless_Y = NULL;
                GtkWidget* Gui_Chunk_Size_Label = NULL;
                GtkWidget* Gui_Chunk_Size_SpinButton = NULL;
            // Bottom Row, master controls
            GtkWidget* Gui_Box_H_Row_Bottom = NULL;
                GtkWidget* Gui_Button_Plugin_Run = NULL;
                GtkWidget* Gui_Button_Plugin_Reset = NULL;
                GtkWidget* Gui_Button_Plugin_Cancel = NULL;

        u16* button_pointer_Size_Reset = NULL;
        u16* button_pointer_m2 = NULL;
        u16* button_pointer_m4 = NULL;
        u16* button_pointer_m5 = NULL;
        u16* button_pointer_m10 = NULL;
        u16* button_pointer_d2 = NULL;
        u16* button_pointer_d4 = NULL;
        u16* button_pointer_d5 = NULL;
        u16* button_pointer_d10 = NULL;
        u16* button_pointer_flip_xy = NULL;
        u16* button_pointer_sample_grid_scale_Reset = NULL;
        u16* button_pointer_sample_grid_scale_125 = NULL;
        u16* button_pointer_sample_grid_scale_150 = NULL;
        u16* button_pointer_sample_grid_scale_200 = NULL;
        u16* button_pointer_sample_grid_scale_250 = NULL;
        u16* button_pointer_sample_grid_scale_325 = NULL;
        u16* button_pointer_sample_grid_scale_500 = NULL;
        u16* button_pointer_sample_grid_scale_1200 = NULL;
        u16* button_pointer_sample_grid_shape_auto = NULL;
        u16* button_pointer_sample_grid_shape_square = NULL;
        u16* button_pointer_sample_grid_shape_circle = NULL;
        u16* button_pointer_sample_interpolation_nearest = NULL;
        u16* button_pointer_sample_interpolation_linear = NULL;
        u16* button_pointer_sample_interpolation_quadratic = NULL;
        u16* button_pointer_sample_interpolation_cubic = NULL;
        u16* button_pointer_sample_interpolation_quartic = NULL;
        u16* button_pointer_sample_interpolation_quintic = NULL;
        u16* button_pointer_seamless_x = NULL;
        u16* button_pointer_seamless_y = NULL;
        u16* button_pointer_run_plugin = NULL;
        u16* button_pointer_cancel_plugin = NULL;
        u16* button_pointer_reset_plugin = NULL;

        ComboSizeWidget* self = NULL;

        bool                    ignore_auto_changes = false;
        gboolean                chain_button_on = (gboolean) TRUE;
        f128                    original_ratio_xy = 1.0_q;
        f128                    locked_ratio_xy = 1.0_q;
        gint                    original_x = (gint) 256;
        gint                    original_y = (gint) 256;
        gint                    size_x = (gint) 256;
        gint                    size_y = (gint) 256;
        gdouble                 scale_x = (gdouble) 1.0;
        gdouble                 scale_y = (gdouble) 1.0;
        gboolean                chain_button_sample_grid_scale_on = (gboolean) TRUE;
        gboolean                seamless_x = (gboolean) FALSE;
        gboolean                seamless_y = (gboolean) FALSE;
        f128                    sample_grid_scale_locked_ratio_xy = 1.0_q;
        gdouble                 sample_grid_scale_x = (gdouble) 1.0;
        gdouble                 sample_grid_scale_y = (gdouble) 1.0;
        gboolean                chain_button_sample_interpolation_on = (gboolean) TRUE;
        gdouble                 sample_interpolation_x = (gdouble) 3.0;
        gdouble                 sample_interpolation_y = (gdouble) 3.0;
        string                  sample_interpolation_x_text = "";
        string                  sample_interpolation_y_text = "";
        f128                    sample_interpolation_locked_ratio_xy = 1.0_q;
        SAMPLE_GRID_SHAPE       sample_grid_shape = SAMPLE_GRID_SHAPE_Auto;
        f128                    sample_grid_weighting = 0._q;
        f128                    sample_count_adjustment = 1._q;
        u64                     chunk_size_kilo = 96uL;

        s8                     choices_done_result = 0;
    };
};
