#pragma once

/*
Copyright (c) Tiger's Eye Jade Swiftwing, all rights reserved.

This file is written by Tiger's Eye Jade Swiftwing.  It is licensed under the GPLv3 license.
Note that my first name is "Tiger's Eye" (which is two words), my middle name is "Jade", and "Swiftwing" is one word that is my last name.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/


#include "config_h/clang64/config.h"

#ifndef GIMP_DISABLE_COMPAR_CRUFT
#define GIMP_DISABLE_COMPAR_CRUFT
#endif

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include <libgimp/stdplugins-intl.h>

#include "components/thp_types.h"
#include "components/thp_parameters.h"
#include "components/thp_log.h"

using namespace TritonhawkPlus;
using namespace std;
using namespace quadmath;

namespace TritonhawkPlus
{
    class ComboSizeWidget
    {
    public:
        ComboSizeWidget(GtkWidget* dialog_in, ThpParams* params_in=NULL, ThpLog* log_in=NULL);
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
        void SetButtonClicked(int* button_name);
        int GetChoicesDoneResult();

    private:
        void UpdateGUI();
        void SyncDataToParameters();
        void SyncDataFromParameters();

        GtkWidget* dialog = NULL;
        ThpParams* Params = NULL;
        ThpLog*    Log = NULL;

        GtkWidget* Gui_Box_Master = NULL;

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

        GtkWidget* Gui_Box_H_Row_1 = NULL;

        GtkWidget* Gui_Button_Size_Reset = NULL;
        GtkWidget* Gui_Button_m2 = NULL;
        GtkWidget* Gui_Button_m5 = NULL;
        GtkWidget* Gui_Button_m10 = NULL;
        GtkWidget* Gui_Button_d2 = NULL;
        GtkWidget* Gui_Button_d5 = NULL;
        GtkWidget* Gui_Button_d10 = NULL;
        GtkWidget* Gui_Button_flip_xy = NULL;

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
        GtkWidget* Gui_Button_SampleGridScale_900 = NULL;

        GtkWidget* Gui_Box_H_Row_3 = NULL;
        GtkWidget* Gui_Button_Seamless_X = NULL;
        GtkWidget* Gui_Button_Seamless_Y = NULL;

        GtkWidget* Gui_Box_H_Row_4 = NULL;
        GtkWidget* Gui_Button_Plugin_Run = NULL;
        GtkWidget* Gui_Button_Plugin_Reset = NULL;
        GtkWidget* Gui_Button_Plugin_Cancel = NULL;

        int* button_pointer_Size_Reset = NULL;
        int* button_pointer_m2 = NULL;
        int* button_pointer_m5 = NULL;
        int* button_pointer_m10 = NULL;
        int* button_pointer_d2 = NULL;
        int* button_pointer_d5 = NULL;
        int* button_pointer_d10 = NULL;
        int* button_pointer_flip_xy = NULL;
        int* button_pointer_sample_grid_scale_Reset = NULL;
        int* button_pointer_sample_grid_scale_125 = NULL;
        int* button_pointer_sample_grid_scale_150 = NULL;
        int* button_pointer_sample_grid_scale_200 = NULL;
        int* button_pointer_sample_grid_scale_250 = NULL;
        int* button_pointer_sample_grid_scale_325 = NULL;
        int* button_pointer_sample_grid_scale_500 = NULL;
        int* button_pointer_sample_grid_scale_900 = NULL;
        int* button_pointer_seamless_x = NULL;
        int* button_pointer_seamless_y = NULL;
        int* button_pointer_run_plugin = NULL;
        int* button_pointer_cancel_plugin = NULL;
        int* button_pointer_reset_plugin = NULL;

        ComboSizeWidget* self = NULL;

        bool                    ignore_auto_changes = false;
        gboolean                chain_button_on = (gboolean) TRUE;
        f128                    original_ratio_xy = 1.0Q;
        f128                    locked_ratio_xy = 1.0Q;
        gint                    original_x = (gint) 256;
        gint                    original_y = (gint) 256;
        gint                    size_x = (gint) 256;
        gint                    size_y = (gint) 256;
        gdouble                 scale_x = (gdouble) 1.0;
        gdouble                 scale_y = (gdouble) 1.0;
        gboolean                chain_button_sample_grid_scale_on = (gboolean) TRUE;
        gboolean                seamless_x = (gboolean) FALSE;
        gboolean                seamless_y = (gboolean) FALSE;
        f128                    sample_grid_scale_locked_ratio_xy = 1.0Q;
        gdouble                 sample_grid_scale_x = (gdouble) 1.0;
        gdouble                 sample_grid_scale_y = (gdouble) 1.0;

        int                     choices_done_result = 0;
    };
};
