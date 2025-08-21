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
#include "components/thp_gui-combowidget-size.h"
#include "components/thp_parameters.h"
#include "components/thp_log.h"

using namespace TritonhawkPlus;
using namespace std;
using namespace quadmath;

namespace TritonhawkPlus
{
    void value_changed_size_x(GtkSpinButton* self, gpointer user_data)
    {
        gint val = gtk_spin_button_get_value_as_int((GtkSpinButton*)self);
        ((ComboSizeWidget*)(user_data))->SetSizeX(val);
    };
    void value_changed_size_y(GtkSpinButton* self, gpointer user_data)
    {
        gint val = gtk_spin_button_get_value_as_int((GtkSpinButton*)self);
        ((ComboSizeWidget*)(user_data))->SetSizeY(val);
    };
    void value_changed_scale_x(GtkSpinButton* self, gpointer user_data)
    {
        gdouble val = gtk_spin_button_get_value((GtkSpinButton*)self);
        ((ComboSizeWidget*)(user_data))->SetScaleX(val);
    };
    void value_changed_scale_y(GtkSpinButton* self, gpointer user_data)
    {
        gdouble val = gtk_spin_button_get_value((GtkSpinButton*)self);
        ((ComboSizeWidget*)(user_data))->SetScaleY(val);
    };
    void value_changed_chain_button(GimpChainButton* self, gpointer user_data)
    {
        gboolean val = gimp_chain_button_get_active((GimpChainButton*)self);
        ((ComboSizeWidget*)(user_data))->SetChainButton(val);
    };
    void value_changed_sample_grid_scale_x(GtkSpinButton* self, gpointer user_data)
    {
        gdouble val = gtk_spin_button_get_value((GtkSpinButton*)self);
        ((ComboSizeWidget*)(user_data))->SetScale_SampleGridScaleX(val);
    };
    void value_changed_sample_grid_scale_y(GtkSpinButton* self, gpointer user_data)
    {
        gdouble val = gtk_spin_button_get_value((GtkSpinButton*)self);
        ((ComboSizeWidget*)(user_data))->SetScale_SampleGridScaleY(val);
    };
    void value_changed_chain_button_sample_grid_scale(GimpChainButton* self, gpointer user_data)
    {
        gboolean val = gimp_chain_button_get_active((GimpChainButton*)self);
        ((ComboSizeWidget*)(user_data))->SetChainButton_SampleGridScale(val);
    };
    void value_changed_sample_weighting(GtkSpinButton* self, gpointer user_data)
    {
        gdouble val = gtk_spin_button_get_value((GtkSpinButton*)self);
        ((ComboSizeWidget*)(user_data))->SetSampleGridWeighting(val);
    };
    void value_changed_sample_count_adj(GtkSpinButton* self, gpointer user_data)
    {
        gdouble val = gtk_spin_button_get_value((GtkSpinButton*)self);
        ((ComboSizeWidget*)(user_data))->SetSampleCountAdj(val);
    };
    void value_changed_interpolation_x(GtkSpinButton* self, gpointer user_data)
    {
        gdouble val = gtk_spin_button_get_value((GtkSpinButton*)self);
        ((ComboSizeWidget*)(user_data))->SetSampleInterpolationX(val);
    };
    void value_changed_interpolation_y(GtkSpinButton* self, gpointer user_data)
    {
        gdouble val = gtk_spin_button_get_value((GtkSpinButton*)self);
        ((ComboSizeWidget*)(user_data))->SetSampleInterpolationY(val);
    };
    void value_changed_chain_button_sample_interpolation(GimpChainButton* self, gpointer user_data)
    {
        gboolean val = gimp_chain_button_get_active((GimpChainButton*)self);
        ((ComboSizeWidget*)(user_data))->SetChainButton_SampleInterpolation(val);
    };
    void value_changed_chunk_kilopixels(GtkSpinButton* self, gpointer user_data)
    {
        gint val = gtk_spin_button_get_value_as_int((GtkSpinButton*)self);
        ((ComboSizeWidget*)(user_data))->SetKilopixels(val);
    };
    void value_changed_button(GimpButton* self, gpointer user_data)
    {
        u16* name = (u16*)g_object_get_data((GObject*)self, "button-name");
        ((ComboSizeWidget*)(user_data))->SetButtonClicked(name);
    };

    void ComboSizeWidget::ShowWidget(GtkWidget* gw)
    {
        if (!gw) return;

        gtk_widget_set_halign(GTK_WIDGET(gw), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(gw), GTK_ALIGN_START);
        gtk_widget_show(GTK_WIDGET(gw));
    }
    ComboSizeWidget::ComboSizeWidget(GtkWidget* Program_Dialog_in, ThpParams* Params_in, ThpLog* Log_in)
    {
        if ((!Program_Dialog_in) || (!Params_in))
            return;
        Program_Dialog = Program_Dialog_in;
        Params = Params_in;
        if (Log_in)
            Log = Log_in;

        gint master_width = (gint) 820;
        gint box_padding = (gint) 2;
        gint cell_height = (gint) 36;
        gint cell_height_large = gint (cell_height + box_padding + box_padding);
        gint cell_height_half = gint (cell_height / 2);
        gint chain_button_width = (gint) 36;
        // Row 0, image size/scale controls
        gint cell_r0_width = gint ((master_width - ((box_padding * 2) + chain_button_width)) / 3);
        gint row_0_height = gint ((cell_height * 2) + cell_height_half);
        // Row 1, image size/scale quickset buttons
        gint cell_r1_width = gint ((master_width - (box_padding * 2)) / 11);
        gint cell_r1_width_half = gint (cell_r1_width / 2);
        gint row_1_height = cell_height;
        // Row 2, sample grid scale controls
        gint cell_r2_width = gint (160 - (box_padding * 2));
        gint row_2_height = gint ((cell_height * 2) + box_padding + box_padding);
        gint sample_grid_quickset_button_group_width = gint (master_width - (cell_r2_width + cell_r2_width + box_padding + box_padding + chain_button_width));
        gint sample_grid_quickset_button_row_width = gint (sample_grid_quickset_button_group_width - (box_padding * 2));
        gint sample_grid_quickset_button_width = gint (sample_grid_quickset_button_row_width / 4);
        // Row 3, sample grid shape controls
        gint row_3_height = cell_height_large;
        gint row_3_shape_label_width = (gint) 180;
        gint cell_r3_width = gint ((master_width - ((box_padding * 2) + row_3_shape_label_width)) / 3);
        // Row 4, sample weighting and count controls
        gint row_4_height = cell_height_large;
        gint row_4_col_width = gint ((master_width - (box_padding * 2)) / 4);
        // Row 5, sample interpolation controls
        gint row_5_height = cell_height_large;
        gint row_5_col_height = cell_height;
        gint row_5_col_width_padded = gint ((master_width - ((box_padding * 2) + chain_button_width)) / 4);
        gint sample_intp_quickset_button_group_width = gint (row_5_col_width_padded * 2);
        gint sample_intp_quickset_button_row_width = gint (row_5_col_width_padded * 2);
        gint sample_intp_quickset_button_width = gint (row_5_col_width_padded * 2 / 3);
        // Row 6, border wrapping controls
        gint row_6_height = cell_height;
        gint row_6_height_large = cell_height_large;
        gint cell_r6_width = gint ((master_width - (box_padding * 2)) / 4);
        // Bottom Row, master controls
        gint row_bottom_button_width = gint ((master_width - (box_padding * 2)) / 3);

        // Master Box
        Gui_Box_Master = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_Master), 5);
        gtk_widget_set_size_request(Gui_Box_Master, master_width, 160);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG (Program_Dialog))), Gui_Box_Master);
        ShowWidget(Gui_Box_Master);

        // Row 0, image size and scale controls
        {
            // First Row Box
            Gui_Box_H_Row_0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_0), box_padding);
            gtk_widget_set_size_request(Gui_Box_H_Row_0, master_width, row_0_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_0, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Row_0);

            // First Row, First Column Box
            Gui_Box_V_Size_Original = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Size_Original), 0);
            gtk_widget_set_size_request(Gui_Box_V_Size_Original, cell_r0_width, row_0_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_0), (GtkWidget*)Gui_Box_V_Size_Original, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_V_Size_Original);
            // Text Label that marks the original image data's dimensions
            Gui_Text_Size_Original_Label = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_Text_Size_Original_Label, cell_r0_width, cell_height_half);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Original), (GtkWidget*)Gui_Text_Size_Original_Label, FALSE, FALSE, 0);
            ShowWidget(Gui_Text_Size_Original_Label);
            gtk_label_set_text((GtkLabel*)Gui_Text_Size_Original_Label, g_strdup_printf(_("%s" "Original Size"), "") );
            // Text Label that marks the original image data's dimensions
            Gui_Text_Size_Original_X = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_Text_Size_Original_X, cell_r0_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Original), (GtkWidget*)Gui_Text_Size_Original_X, FALSE, FALSE, 0);
            ShowWidget(Gui_Text_Size_Original_X);
            gtk_label_set_text((GtkLabel*)Gui_Text_Size_Original_X, g_strdup_printf(_("Width: %i" "%s"), original_x, "") );
            // Text Label that marks the original image data's dimensions
            Gui_Text_Size_Original_Y = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_Text_Size_Original_Y, cell_r0_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Original), (GtkWidget*)Gui_Text_Size_Original_Y, FALSE, FALSE, 0);
            ShowWidget(Gui_Text_Size_Original_Y);
            gtk_label_set_text((GtkLabel*)Gui_Text_Size_Original_Y, g_strdup_printf(_("Height: %i" "%s"), original_y, "") );

            // First Row, Second Column Box
            Gui_Box_V_Size_Pixels = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Size_Pixels), 0);
            gtk_widget_set_size_request(Gui_Box_V_Size_Pixels, cell_r0_width, row_0_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_0), (GtkWidget*)Gui_Box_V_Size_Pixels, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_V_Size_Pixels);
            // Text Label that marks the new image data's dimensions
            Gui_Text_Size_New_Pixels_Label = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_Text_Size_New_Pixels_Label, cell_r0_width, cell_height_half);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Pixels), (GtkWidget*)Gui_Text_Size_New_Pixels_Label, FALSE, FALSE, 0);
            ShowWidget(Gui_Text_Size_New_Pixels_Label);
            gtk_label_set_text((GtkLabel*)Gui_Text_Size_New_Pixels_Label, g_strdup_printf(_("%s" "New Size (Pixels)"), "") );
            // Spin Button that determines the new image data's dimensions
            Gui_SpinButton_Size_X_Pixels = gimp_spin_button_new_with_range((gdouble) 1.0, gdouble (Params->max_image_dimension), (gdouble) 1.0);
            gtk_widget_set_size_request(Gui_SpinButton_Size_X_Pixels, cell_r0_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Pixels), (GtkWidget*)Gui_SpinButton_Size_X_Pixels, FALSE, FALSE, 0);
            gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Size_X_Pixels, (guint) 0);
            ShowWidget(Gui_SpinButton_Size_X_Pixels);
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Size_X_Pixels, (gdouble)size_x);
            // Spin Button that determines the new image data's dimensions
            Gui_SpinButton_Size_Y_Pixels = gimp_spin_button_new_with_range((gdouble) 1.0, gdouble (Params->max_image_dimension), (gdouble) 1.0);
            gtk_widget_set_size_request(Gui_SpinButton_Size_Y_Pixels, cell_r0_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Pixels), (GtkWidget*)Gui_SpinButton_Size_Y_Pixels, FALSE, FALSE, 0);
            gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Size_Y_Pixels, (guint) 0);
            ShowWidget(Gui_SpinButton_Size_Y_Pixels);
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Size_Y_Pixels, (gdouble)size_y);

            // First Row, Third Column Box
            Gui_Box_V_Size_Scale = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Size_Scale), 0);
            gtk_widget_set_size_request(Gui_Box_V_Size_Scale, cell_r0_width, row_0_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_0), (GtkWidget*)Gui_Box_V_Size_Scale, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_V_Size_Scale);
            // Text Label that marks the new image data's scale
            Gui_Text_Size_New_Scale_Label = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_Text_Size_New_Scale_Label, cell_r0_width, cell_height_half);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Scale), (GtkWidget*)Gui_Text_Size_New_Scale_Label, FALSE, FALSE, 0);
            ShowWidget(Gui_Text_Size_New_Scale_Label);
            gtk_label_set_text((GtkLabel*)Gui_Text_Size_New_Scale_Label, g_strdup_printf(_("%s" "New Scale (Percent)"), "") );
            // Spin Button that determines the new image data's scale
            Gui_SpinButton_Size_X_Scale = gimp_spin_button_new_with_range((gdouble) 0.0001, (gdouble) 99999999.9999, (gdouble) 1.0);
            gtk_widget_set_size_request(Gui_SpinButton_Size_X_Scale, cell_r0_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Scale), (GtkWidget*)Gui_SpinButton_Size_X_Scale, FALSE, FALSE, 0);
            gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Size_X_Scale, (guint) 4u);
            ShowWidget(Gui_SpinButton_Size_X_Scale);
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Size_X_Scale, (gdouble)scale_x * (gdouble)100.0);
            // Spin Button that determines the new image data's scale
            Gui_SpinButton_Size_Y_Scale = gimp_spin_button_new_with_range((gdouble) 0.0001, (gdouble) 99999999.9999, (gdouble) 1.0);
            gtk_widget_set_size_request(Gui_SpinButton_Size_Y_Scale, cell_r0_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Scale), (GtkWidget*)Gui_SpinButton_Size_Y_Scale, FALSE, FALSE, 0);
            gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Size_Y_Scale, (guint) 4u);
            ShowWidget(Gui_SpinButton_Size_Y_Scale);
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Size_Y_Scale, (gdouble)scale_y * (gdouble)100.0);

            // First Row, Fourth Column Box
            Gui_Box_V_Chainbutton = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Chainbutton), 0);
            gtk_widget_set_size_request(Gui_Box_V_Chainbutton, chain_button_width, row_0_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_0), (GtkWidget*)Gui_Box_V_Chainbutton, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_V_Chainbutton);
            // First Row, Fourth Column Box Spacer, empty space above chain button
            Gui_Box_V_Spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Spacer), 0);
            gtk_widget_set_size_request(Gui_Box_V_Spacer, chain_button_width, cell_height_half);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Chainbutton), (GtkWidget*)Gui_Box_V_Spacer, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_V_Spacer);
            // Chain button that links X and Y dimensions to optionally maintain ratio
            Gui_ChainButton = gimp_chain_button_new(GIMP_CHAIN_RIGHT);
            gtk_widget_set_size_request(Gui_ChainButton, chain_button_width, cell_height + cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Chainbutton), (GtkWidget*)Gui_ChainButton, FALSE, FALSE, 0);
            ShowWidget(Gui_ChainButton);
        }

        // Row 1, image size and scale controls
        {
            // Row 1 Box
            Gui_Box_H_Row_1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_1), box_padding);
            gtk_widget_set_size_request(Gui_Box_H_Row_1, master_width, row_1_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_1, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Row_1);

            // Button, Reset
            Gui_Button_Size_Reset = gtk_button_new_with_label("Reset");
            gtk_widget_set_size_request(Gui_Button_Size_Reset, cell_r1_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_Size_Reset, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Size_Reset);
            // Button, Swap X and Y sizes
            Gui_Button_flip_xy = gtk_button_new_with_label("Flip X-Y");
            gtk_widget_set_size_request(Gui_Button_flip_xy, cell_r1_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_flip_xy, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_flip_xy);

            // Row 1 Spacer 1
            Gui_Box_H_Row_1_Spacer_1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_1_Spacer_1), 0);
            gtk_widget_set_size_request(Gui_Box_H_Row_1_Spacer_1, cell_r1_width_half, cell_height_half);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Box_H_Row_1_Spacer_1, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Row_1_Spacer_1);

            // Button, Multiply by 2
            Gui_Button_m2 = gtk_button_new_with_label("x 2");
            gtk_widget_set_size_request(Gui_Button_m2, cell_r1_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_m2, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_m2);
            // Button, Multiply by 4
            Gui_Button_m4 = gtk_button_new_with_label("x 4");
            gtk_widget_set_size_request(Gui_Button_m4, cell_r1_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_m4, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_m4);
            // Button, Multiply by 5
            Gui_Button_m5 = gtk_button_new_with_label("x 5");
            gtk_widget_set_size_request(Gui_Button_m5, cell_r1_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_m5, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_m5);
            // Button, Multiply by 10
            Gui_Button_m10 = gtk_button_new_with_label("x 10");
            gtk_widget_set_size_request(Gui_Button_m10, cell_r1_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_m10, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_m10);

            // Row 1 Spacer 2
            Gui_Box_H_Row_1_Spacer_2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_1_Spacer_2), 0);
            gtk_widget_set_size_request(Gui_Box_H_Row_1_Spacer_2, cell_r1_width_half, cell_height_half);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Box_H_Row_1_Spacer_2, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Row_1_Spacer_2);

            // Button, Divide by 2
            Gui_Button_d2 = gtk_button_new_with_label("x 1/2");
            gtk_widget_set_size_request(Gui_Button_d2, cell_r1_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_d2, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_d2);
            // Button, Divide by 4
            Gui_Button_d4 = gtk_button_new_with_label("x 1/4");
            gtk_widget_set_size_request(Gui_Button_d4, cell_r1_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_d4, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_d4);
            // Button, Divide by 5
            Gui_Button_d5 = gtk_button_new_with_label("x 1/5");
            gtk_widget_set_size_request(Gui_Button_d5, cell_r1_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_d5, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_d5);
            // Button, Divide by 10
            Gui_Button_d10 = gtk_button_new_with_label("x 1/10");
            gtk_widget_set_size_request(Gui_Button_d10, cell_r1_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_d10, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_d10);
        }

        // Row 2, sample grid scale controls
        {
            // Row 2 Box
            Gui_Box_H_Row_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_2), box_padding);
            gtk_widget_set_size_request(Gui_Box_H_Row_2, master_width, row_2_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_2, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Row_2);

            // Row 2, Column 0 Box
            Gui_Box_V_Sample_Grid_Scale_Labels = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Sample_Grid_Scale_Labels), box_padding);
            gtk_widget_set_size_request(Gui_Box_V_Sample_Grid_Scale_Labels, cell_r2_width, row_2_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_2), (GtkWidget*)Gui_Box_V_Sample_Grid_Scale_Labels, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_V_Sample_Grid_Scale_Labels);
            // Sample Grid Scale Label, X
            Gui_SpinButton_Sample_Grid_Scale_X_Label = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_SpinButton_Sample_Grid_Scale_X_Label, cell_r2_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Grid_Scale_Labels), (GtkWidget*)Gui_SpinButton_Sample_Grid_Scale_X_Label, FALSE, FALSE, 0);
            ShowWidget(Gui_SpinButton_Sample_Grid_Scale_X_Label);
            gtk_label_set_text((GtkLabel*)Gui_SpinButton_Sample_Grid_Scale_X_Label, g_strdup_printf(_("Sample Grid Scale X (Percent)" "%s"), "") );
            // Sample Grid Scale Label, Y
            Gui_SpinButton_Sample_Grid_Scale_Y_Label = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_SpinButton_Sample_Grid_Scale_Y_Label, cell_r2_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Grid_Scale_Labels), (GtkWidget*)Gui_SpinButton_Sample_Grid_Scale_Y_Label, FALSE, FALSE, 0);
            ShowWidget(Gui_SpinButton_Sample_Grid_Scale_Y_Label);
            gtk_label_set_text((GtkLabel*)Gui_SpinButton_Sample_Grid_Scale_Y_Label, g_strdup_printf(_("Sample Grid Scale Y (Percent)" "%s"), "") );

            // Row 2, Column 1 Box
            Gui_Box_V_Sample_Grid_Scale_SpinButtons = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Sample_Grid_Scale_SpinButtons), 0);
            gtk_widget_set_size_request(Gui_Box_V_Sample_Grid_Scale_SpinButtons, cell_r2_width, row_2_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_2), (GtkWidget*)Gui_Box_V_Sample_Grid_Scale_SpinButtons, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_V_Sample_Grid_Scale_SpinButtons);
            // Spin Button that determines the sample grid's scale, X
            Gui_SpinButton_Sample_Grid_Scale_X = gimp_spin_button_new_with_range((gdouble) 0.01, (gdouble) Params->max_sample_grid_dimension_percent, (gdouble) 1.00);
            gtk_widget_set_size_request(Gui_SpinButton_Sample_Grid_Scale_X, cell_r2_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Grid_Scale_SpinButtons), (GtkWidget*)Gui_SpinButton_Sample_Grid_Scale_X, FALSE, FALSE, 0);
            gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Sample_Grid_Scale_X, (guint) 2u);
            ShowWidget(Gui_SpinButton_Sample_Grid_Scale_X);
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Sample_Grid_Scale_X, (gdouble)sample_grid_scale_x * (gdouble)100.0);
            // Spin Button that determines the sample grid's scale, Y
            Gui_SpinButton_Sample_Grid_Scale_Y = gimp_spin_button_new_with_range((gdouble) 0.01, (gdouble) Params->max_sample_grid_dimension_percent, (gdouble) 1.00);
            gtk_widget_set_size_request(Gui_SpinButton_Sample_Grid_Scale_Y, cell_r2_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Grid_Scale_SpinButtons), (GtkWidget*)Gui_SpinButton_Sample_Grid_Scale_Y, FALSE, FALSE, 0);
            gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Sample_Grid_Scale_Y, (guint) 2u);
            ShowWidget(Gui_SpinButton_Sample_Grid_Scale_Y);
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Sample_Grid_Scale_Y, (gdouble)sample_grid_scale_y * (gdouble)100.0);
            // Chain Button that determines when the sample grid's scale has it's aspect ratio locked
            Gui_ChainButton_Sample_Grid_Scale = gimp_chain_button_new(GIMP_CHAIN_RIGHT);
            gtk_widget_set_size_request(Gui_ChainButton_Sample_Grid_Scale, chain_button_width, cell_height * (gint)2);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_2), (GtkWidget*)Gui_ChainButton_Sample_Grid_Scale, FALSE, FALSE, 0);
            ShowWidget(Gui_ChainButton_Sample_Grid_Scale);

            // Sample Grid Scale Quickset Buttons
            Gui_Box_V_Sample_Grid_Scale_QuickSetButtons = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Sample_Grid_Scale_QuickSetButtons), 0);
            gtk_widget_set_size_request(Gui_Box_V_Sample_Grid_Scale_QuickSetButtons, sample_grid_quickset_button_group_width, row_2_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_2), (GtkWidget*)Gui_Box_V_Sample_Grid_Scale_QuickSetButtons, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_V_Sample_Grid_Scale_QuickSetButtons);
            // Sample Grid Scale Quickset Buttons, Row 0
            Gui_Box_H_Sample_Grid_Scale_QuickSet_0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Sample_Grid_Scale_QuickSet_0), 0);
            gtk_widget_set_size_request(Gui_Box_H_Sample_Grid_Scale_QuickSet_0, sample_grid_quickset_button_row_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Grid_Scale_QuickSetButtons), (GtkWidget*)Gui_Box_H_Sample_Grid_Scale_QuickSet_0, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Sample_Grid_Scale_QuickSet_0);
            // Sample Grid Scale Quickset Button
            Gui_Button_SampleGridScale_Reset = gtk_button_new_with_label("Reset");
            gtk_widget_set_size_request(Gui_Button_SampleGridScale_Reset, sample_grid_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_0), (GtkWidget*)Gui_Button_SampleGridScale_Reset, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_SampleGridScale_Reset);
            // Sample Grid Scale Quickset Button
            Gui_Button_SampleGridScale_125 = gtk_button_new_with_label("%125");
            gtk_widget_set_size_request(Gui_Button_SampleGridScale_125, sample_grid_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_0), (GtkWidget*)Gui_Button_SampleGridScale_125, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_SampleGridScale_125);
            // Sample Grid Scale Quickset Button
            Gui_Button_SampleGridScale_150 = gtk_button_new_with_label("%150");
            gtk_widget_set_size_request(Gui_Button_SampleGridScale_150, sample_grid_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_0), (GtkWidget*)Gui_Button_SampleGridScale_150, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_SampleGridScale_150);
            // Sample Grid Scale Quickset Button
            Gui_Button_SampleGridScale_200 = gtk_button_new_with_label("%200");
            gtk_widget_set_size_request(Gui_Button_SampleGridScale_200, sample_grid_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_0), (GtkWidget*)Gui_Button_SampleGridScale_200, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_SampleGridScale_200);
            // Sample Grid Scale Quickset Buttons, Row 1
            Gui_Box_H_Sample_Grid_Scale_QuickSet_1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Sample_Grid_Scale_QuickSet_1), 0);
            gtk_widget_set_size_request(Gui_Box_H_Sample_Grid_Scale_QuickSet_1, sample_grid_quickset_button_row_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Grid_Scale_QuickSetButtons), (GtkWidget*)Gui_Box_H_Sample_Grid_Scale_QuickSet_1, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Sample_Grid_Scale_QuickSet_1);
            // Sample Grid Scale Quickset Button
            Gui_Button_SampleGridScale_250 = gtk_button_new_with_label("%250");
            gtk_widget_set_size_request(Gui_Button_SampleGridScale_250, sample_grid_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_1), (GtkWidget*)Gui_Button_SampleGridScale_250, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_SampleGridScale_250);
            // Sample Grid Scale Quickset Button
            Gui_Button_SampleGridScale_325 = gtk_button_new_with_label("%325");
            gtk_widget_set_size_request(Gui_Button_SampleGridScale_325, sample_grid_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_1), (GtkWidget*)Gui_Button_SampleGridScale_325, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_SampleGridScale_325);
            // Sample Grid Scale Quickset Button
            Gui_Button_SampleGridScale_500 = gtk_button_new_with_label("%500");
            gtk_widget_set_size_request(Gui_Button_SampleGridScale_500, sample_grid_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_1), (GtkWidget*)Gui_Button_SampleGridScale_500, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_SampleGridScale_500);
            // Sample Grid Scale Quickset Button
            Gui_Button_SampleGridScale_1200 = gtk_button_new_with_label("%1200");
            gtk_widget_set_size_request(Gui_Button_SampleGridScale_1200, sample_grid_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_1), (GtkWidget*)Gui_Button_SampleGridScale_1200, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_SampleGridScale_1200);
        }

        // Row 3, sample grid shape controls
        {
            // Row 3 Box
            Gui_Box_H_Row_3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_3), box_padding);
            gtk_widget_set_size_request(Gui_Box_H_Row_3, master_width, row_3_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_3, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Row_3);

            // Sample Grid Shape Text
            Gui_Sample_Grid_Shape_Label = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_Sample_Grid_Shape_Label, row_3_shape_label_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_3), (GtkWidget*)Gui_Sample_Grid_Shape_Label, FALSE, FALSE, 0);
            ShowWidget(Gui_Sample_Grid_Shape_Label);
            gtk_label_set_text((GtkLabel*)Gui_Sample_Grid_Shape_Label, g_strdup_printf(_("Sample Grid Shape:\n" "%s"), " Auto"));

            // Sample Grid Shape Button, Auto
            Gui_Button_Grid_Shape_Auto = gtk_button_new_with_label("Auto");
            gtk_widget_set_size_request(Gui_Button_Grid_Shape_Auto, cell_r3_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_3), (GtkWidget*)Gui_Button_Grid_Shape_Auto, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Grid_Shape_Auto);
            // Sample Grid Shape Button, Square
            Gui_Button_Grid_Shape_Square = gtk_button_new_with_label("Square");
            gtk_widget_set_size_request(Gui_Button_Grid_Shape_Square, cell_r3_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_3), (GtkWidget*)Gui_Button_Grid_Shape_Square, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Grid_Shape_Square);
            // Sample Grid Shape Button, Circle
            Gui_Button_Grid_Shape_Circle = gtk_button_new_with_label("Circle");
            gtk_widget_set_size_request(Gui_Button_Grid_Shape_Circle, cell_r3_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_3), (GtkWidget*)Gui_Button_Grid_Shape_Circle, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Grid_Shape_Circle);
        }

        // Row 4, sample weighting and count controls
        {
            // Row 4 Box
            Gui_Box_H_Row_4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_4), box_padding);
            gtk_widget_set_size_request(Gui_Box_H_Row_4, master_width, row_4_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_4, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Row_4);

            // Text Label, for sample weighting
            Gui_Sample_Weighting_Label = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_Sample_Weighting_Label, row_4_col_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_4), (GtkWidget*)Gui_Sample_Weighting_Label, FALSE, FALSE, 0);
            ShowWidget(Gui_Sample_Weighting_Label);
            gtk_label_set_text((GtkLabel*)Gui_Sample_Weighting_Label, g_strdup_printf(_("Sample Weighting\n" "%s"), " Off") );
            // Spin Button for sample weights
            Gui_SpinButton_Sample_Weighting = gimp_spin_button_new_with_range((gdouble) 0.0, (gdouble) 1.0, (gdouble) 0.01);
            gtk_widget_set_size_request(Gui_SpinButton_Sample_Weighting, row_4_col_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_4), (GtkWidget*)Gui_SpinButton_Sample_Weighting, FALSE, FALSE, 0);
            gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Sample_Weighting, (guint) 5u);
            ShowWidget(Gui_SpinButton_Sample_Weighting);
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Sample_Weighting, (gdouble) sample_grid_weighting);

            // Text Label, for sample count adjustment
            Gui_Sample_Count_Label = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_Sample_Count_Label, row_4_col_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_4), (GtkWidget*)Gui_Sample_Count_Label, FALSE, FALSE, 0);
            ShowWidget(Gui_Sample_Count_Label);
            gtk_label_set_text((GtkLabel*)Gui_Sample_Count_Label, g_strdup_printf(_("Sample Count\n" "Adjustment%s"), "") );
            // Spin Button for sample count adjustment
            Gui_SpinButton_Sample_Count = gimp_spin_button_new_with_range((gdouble) 100.0, (gdouble) Params->max_sample_count_modifier, (gdouble) 1.0);
            gtk_widget_set_size_request(Gui_SpinButton_Sample_Count, row_4_col_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_4), (GtkWidget*)Gui_SpinButton_Sample_Count, FALSE, FALSE, 0);
            gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Sample_Count, (guint) 3u);
            ShowWidget(Gui_SpinButton_Sample_Count);
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Sample_Count, gdouble (sample_count_adjustment * 100._q));
        }

        // Row 5, sample interpolation controls
        {
            // Row 5 Box
            Gui_Box_H_Row_5 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_5), box_padding);
            gtk_widget_set_size_request(Gui_Box_H_Row_5, master_width, row_5_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_5, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Row_5);

            // Row 5, Column 0 Box
            Gui_Box_V_Sample_Interpolation_Labels = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Sample_Interpolation_Labels), 0);
            gtk_widget_set_size_request(Gui_Box_V_Sample_Interpolation_Labels, row_5_col_width_padded, row_5_col_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_5), (GtkWidget*)Gui_Box_V_Sample_Interpolation_Labels, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_V_Sample_Interpolation_Labels);
            // Sample Interpolation Label, X
            Gui_Sample_Interpolation_X_Label = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_Sample_Interpolation_X_Label, row_5_col_width_padded, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Interpolation_Labels), (GtkWidget*)Gui_Sample_Interpolation_X_Label, FALSE, FALSE, 0);
            ShowWidget(Gui_Sample_Interpolation_X_Label);
            gtk_label_set_text((GtkLabel*)Gui_Sample_Interpolation_X_Label, g_strdup_printf(_("Sample Interpolation X" "%s"), "") );
            // Sample Interpolation Label, Y
            Gui_Sample_Interpolation_Y_Label = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_Sample_Interpolation_Y_Label, row_5_col_width_padded, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Interpolation_Labels), (GtkWidget*)Gui_Sample_Interpolation_Y_Label, FALSE, FALSE, 0);
            ShowWidget(Gui_Sample_Interpolation_Y_Label);
            gtk_label_set_text((GtkLabel*)Gui_Sample_Interpolation_Y_Label, g_strdup_printf(_("Sample Interpolation Y" "%s"), "") );

            // Row 5, Column 1 Box
            Gui_Box_V_Sample_Interpolation_SpinButtons = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Sample_Interpolation_SpinButtons), 0);
            gtk_widget_set_size_request(Gui_Box_V_Sample_Interpolation_SpinButtons, row_5_col_width_padded, row_5_col_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_5), (GtkWidget*)Gui_Box_V_Sample_Interpolation_SpinButtons, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_V_Sample_Interpolation_SpinButtons);
            // Spin Button that determines the sample interpolation, X
            Gui_Sample_Interpolation_X_SpinButton = gimp_spin_button_new_with_range((gdouble) 0.0, (gdouble) Params->max_sample_interpolation, (gdouble) 0.05);
            gtk_widget_set_size_request(Gui_Sample_Interpolation_X_SpinButton, row_5_col_width_padded, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Interpolation_SpinButtons), (GtkWidget*)Gui_Sample_Interpolation_X_SpinButton, FALSE, FALSE, 0);
            gtk_spin_button_set_digits((GtkSpinButton*)Gui_Sample_Interpolation_X_SpinButton, (guint) 4u);
            ShowWidget(Gui_Sample_Interpolation_X_SpinButton);
            gtk_spin_button_set_value((GtkSpinButton*)Gui_Sample_Interpolation_X_SpinButton, (gdouble)sample_interpolation_x);
            // Spin Button that determines the sample interpolation, Y
            Gui_Sample_Interpolation_Y_SpinButton = gimp_spin_button_new_with_range((gdouble) 0.0, (gdouble) Params->max_sample_interpolation, (gdouble) 0.05);
            gtk_widget_set_size_request(Gui_Sample_Interpolation_Y_SpinButton, row_5_col_width_padded, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Interpolation_SpinButtons), (GtkWidget*)Gui_Sample_Interpolation_Y_SpinButton, FALSE, FALSE, 0);
            gtk_spin_button_set_digits((GtkSpinButton*)Gui_Sample_Interpolation_Y_SpinButton, (guint) 4u);
            ShowWidget(Gui_Sample_Interpolation_Y_SpinButton);
            gtk_spin_button_set_value((GtkSpinButton*)Gui_Sample_Interpolation_Y_SpinButton, (gdouble)sample_interpolation_y);
            // Chain Button that determines when the sample interpolation has it's aspect ratio locked
            Gui_Sample_Interpolation_ChainButton = gimp_chain_button_new(GIMP_CHAIN_RIGHT);
            gtk_widget_set_size_request(Gui_Sample_Interpolation_ChainButton, chain_button_width, cell_height * (gint)2);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_5), (GtkWidget*)Gui_Sample_Interpolation_ChainButton, FALSE, FALSE, 0);
            ShowWidget(Gui_Sample_Interpolation_ChainButton);

            // Row 5, Column 2 Box
            Gui_Box_V_Sample_Interpolation_Quickset = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Sample_Interpolation_Quickset), 0);
            gtk_widget_set_size_request(Gui_Box_V_Sample_Interpolation_Quickset, sample_intp_quickset_button_group_width, row_5_col_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_5), (GtkWidget*)Gui_Box_V_Sample_Interpolation_Quickset, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_V_Sample_Interpolation_Quickset);
            // Sample Interpolation Quickset Buttons, Row 5, Col 2, Cell Row 0
            Gui_Box_H_Sample_Interpolation_Quickset_0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Sample_Interpolation_Quickset_0), 0);
            gtk_widget_set_size_request(Gui_Box_H_Sample_Interpolation_Quickset_0, sample_intp_quickset_button_row_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Interpolation_Quickset), (GtkWidget*)Gui_Box_H_Sample_Interpolation_Quickset_0, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Sample_Interpolation_Quickset_0);
            // Sample Interpolation Button, "Nearest"
            Gui_Button_Sample_Lerp_Quickset_Nearest = gtk_button_new_with_label("Nearest");
            gtk_widget_set_size_request(Gui_Button_Sample_Lerp_Quickset_Nearest, sample_intp_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Interpolation_Quickset_0), (GtkWidget*)Gui_Button_Sample_Lerp_Quickset_Nearest, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Sample_Lerp_Quickset_Nearest);
            // Sample Interpolation Button, "Linear"
            Gui_Button_Sample_Lerp_Quickset_Linear = gtk_button_new_with_label("Linear");
            gtk_widget_set_size_request(Gui_Button_Sample_Lerp_Quickset_Linear, sample_intp_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Interpolation_Quickset_0), (GtkWidget*)Gui_Button_Sample_Lerp_Quickset_Linear, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Sample_Lerp_Quickset_Linear);
            // Sample Interpolation Button, "Quadratic"
            Gui_Button_Sample_Lerp_Quickset_Quadratic = gtk_button_new_with_label("Quadratic");
            gtk_widget_set_size_request(Gui_Button_Sample_Lerp_Quickset_Quadratic, sample_intp_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Interpolation_Quickset_0), (GtkWidget*)Gui_Button_Sample_Lerp_Quickset_Quadratic, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Sample_Lerp_Quickset_Quadratic);
            // Sample Interpolation Quickset Buttons, Row 5, Col 2, Cell Row 1
            Gui_Box_H_Sample_Interpolation_Quickset_1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Sample_Interpolation_Quickset_1), 0);
            gtk_widget_set_size_request(Gui_Box_H_Sample_Interpolation_Quickset_1, sample_intp_quickset_button_row_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Interpolation_Quickset), (GtkWidget*)Gui_Box_H_Sample_Interpolation_Quickset_1, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Sample_Interpolation_Quickset_1);
            // Sample Interpolation Button, "Cubic"
            Gui_Button_Sample_Lerp_Quickset_Cubic = gtk_button_new_with_label("Cubic");
            gtk_widget_set_size_request(Gui_Button_Sample_Lerp_Quickset_Cubic, sample_intp_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Interpolation_Quickset_1), (GtkWidget*)Gui_Button_Sample_Lerp_Quickset_Cubic, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Sample_Lerp_Quickset_Cubic);
            // Sample Interpolation Button, "Quartic"
            Gui_Button_Sample_Lerp_Quickset_Quartic = gtk_button_new_with_label("Quartic");
            gtk_widget_set_size_request(Gui_Button_Sample_Lerp_Quickset_Quartic, sample_intp_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Interpolation_Quickset_1), (GtkWidget*)Gui_Button_Sample_Lerp_Quickset_Quartic, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Sample_Lerp_Quickset_Quartic);
            // Sample Interpolation Button, "Quintic"
            Gui_Button_Sample_Lerp_Quickset_Quintic = gtk_button_new_with_label("Quintic");
            gtk_widget_set_size_request(Gui_Button_Sample_Lerp_Quickset_Quintic, sample_intp_quickset_button_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Interpolation_Quickset_1), (GtkWidget*)Gui_Button_Sample_Lerp_Quickset_Quintic, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Sample_Lerp_Quickset_Quintic);
        }

        // Row 6, border wrapping controls
        {
            // Row 6 Box
            Gui_Box_H_Row_6 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_6), box_padding);
            gtk_widget_set_size_request(Gui_Box_H_Row_6, master_width, row_6_height_large);
            gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_6, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Row_6);

            // Seamless Looping Border Button
            Gui_Button_Seamless_X = gtk_button_new_with_label("Regular Horizontal Borders");
            gtk_widget_set_size_request(Gui_Button_Seamless_X, cell_r6_width, row_6_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_6), (GtkWidget*)Gui_Button_Seamless_X, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Seamless_X);
            // Seamless Looping Border Button
            Gui_Button_Seamless_Y = gtk_button_new_with_label("Regular Vertical Borders");
            gtk_widget_set_size_request(Gui_Button_Seamless_Y, cell_r6_width, row_6_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_6), (GtkWidget*)Gui_Button_Seamless_Y, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Seamless_Y);

            // Text Label, chunk size in kilopixels
            Gui_Chunk_Size_Label = gtk_label_new(NULL);
            gtk_widget_set_size_request(Gui_Chunk_Size_Label, row_4_col_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_6), (GtkWidget*)Gui_Chunk_Size_Label, FALSE, FALSE, 0);
            ShowWidget(Gui_Chunk_Size_Label);
            gtk_label_set_text((GtkLabel*)Gui_Chunk_Size_Label, g_strdup_printf(_("Chunk Size, Kilosamples\n" "%I64u Ksmp, %I64u smp"), chunk_size_kilo, chunk_size_kilo * 1024uL) );
            // Spin Button for kilopixels (chunk size)
            Gui_Chunk_Size_SpinButton = gimp_spin_button_new_with_range((gdouble) Params->min_chunk_size, (gdouble) Params->max_chunk_size, (gdouble) Params->increment_chunk_size);
            gtk_widget_set_size_request(Gui_Chunk_Size_SpinButton, row_4_col_width, cell_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_6), (GtkWidget*)Gui_Chunk_Size_SpinButton, FALSE, FALSE, 0);
            gtk_spin_button_set_digits((GtkSpinButton*)Gui_Chunk_Size_SpinButton, (guint) 0u);
            ShowWidget(Gui_Chunk_Size_SpinButton);
            gtk_spin_button_set_value((GtkSpinButton*)Gui_Chunk_Size_SpinButton, gdouble (chunk_size_kilo));
        }

        // Bottom Row, master controls
        {
            // Bottom Row Box
            Gui_Box_H_Row_Bottom = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
            gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_Bottom), box_padding);
            gtk_widget_set_size_request(Gui_Box_H_Row_Bottom, master_width, row_5_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_Bottom, FALSE, FALSE, 0);
            ShowWidget(Gui_Box_H_Row_Bottom);
            // Plug-in Choice Button
            Gui_Button_Plugin_Run = gtk_button_new_with_label("Run");
            gtk_widget_set_size_request(Gui_Button_Plugin_Run, row_bottom_button_width, row_5_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_Bottom), (GtkWidget*)Gui_Button_Plugin_Run, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Plugin_Run);
            // Plug-in Choice Button
            Gui_Button_Plugin_Cancel = gtk_button_new_with_label("Cancel / Quit");
            gtk_widget_set_size_request(Gui_Button_Plugin_Cancel, row_bottom_button_width, row_5_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_Bottom), (GtkWidget*)Gui_Button_Plugin_Cancel, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Plugin_Cancel);
            // Plug-in Choice Button
            Gui_Button_Plugin_Reset = gtk_button_new_with_label("Full Reset");
            gtk_widget_set_size_request(Gui_Button_Plugin_Reset, row_bottom_button_width, row_5_height);
            gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_Bottom), (GtkWidget*)Gui_Button_Plugin_Reset, FALSE, FALSE, 0);
            ShowWidget(Gui_Button_Plugin_Reset);
        }

        gtk_widget_show(Program_Dialog);

        // ComboSizeWidget_Instance = this;
        self = this;

        // Row 1, image size and scale controls
        button_pointer_Size_Reset = new u16(1u);
        button_pointer_flip_xy = new u16(2u);
        button_pointer_m2 = new u16(11u);
        button_pointer_m4 = new u16(12u);
        button_pointer_m5 = new u16(13u);
        button_pointer_m10 = new u16(14u);
        button_pointer_d2 = new u16(21u);
        button_pointer_d4 = new u16(22u);
        button_pointer_d5 = new u16(23u);
        button_pointer_d10 = new u16(24u);
        // Row 2, sample grid scale controls
        button_pointer_sample_grid_scale_Reset = new u16(41u);
        button_pointer_sample_grid_scale_125 = new u16(42u);
        button_pointer_sample_grid_scale_150 = new u16(43u);
        button_pointer_sample_grid_scale_200 = new u16(44u);
        button_pointer_sample_grid_scale_250 = new u16(45u);
        button_pointer_sample_grid_scale_325 = new u16(46u);
        button_pointer_sample_grid_scale_500 = new u16(47u);
        button_pointer_sample_grid_scale_1200 = new u16(48u);
        // Row 3, sample grid shape controls
        button_pointer_sample_grid_shape_auto = new u16(61u);
        button_pointer_sample_grid_shape_square = new u16(62u);
        button_pointer_sample_grid_shape_circle = new u16(63u);
        // Row 5, sample interpolation controls
        button_pointer_sample_interpolation_nearest = new u16(121u);
        button_pointer_sample_interpolation_linear = new u16(122u);
        button_pointer_sample_interpolation_quadratic = new u16(123u);
        button_pointer_sample_interpolation_cubic = new u16(124u);
        button_pointer_sample_interpolation_quartic = new u16(125u);
        button_pointer_sample_interpolation_quintic = new u16(126u);
        // Row 6, border wrapping controls
        button_pointer_seamless_x = new u16(4501u);
        button_pointer_seamless_y = new u16(4502u);
        // Bottom Row, master controls
        button_pointer_run_plugin = new u16(5001u);
        button_pointer_cancel_plugin = new u16(5002u);
        button_pointer_reset_plugin = new u16(5003u);

        g_signal_connect(Gui_SpinButton_Size_X_Pixels, "value-changed", G_CALLBACK(value_changed_size_x), self);
        g_signal_connect(Gui_SpinButton_Size_Y_Pixels, "value-changed", G_CALLBACK(value_changed_size_y), self);
        g_signal_connect(Gui_SpinButton_Size_X_Scale, "value-changed", G_CALLBACK(value_changed_scale_x), self);
        g_signal_connect(Gui_SpinButton_Size_Y_Scale, "value-changed", G_CALLBACK(value_changed_scale_y), self);
        g_signal_connect(Gui_ChainButton, "toggled", G_CALLBACK(value_changed_chain_button), self);
        g_signal_connect(Gui_SpinButton_Sample_Grid_Scale_X, "value-changed", G_CALLBACK(value_changed_sample_grid_scale_x), self);
        g_signal_connect(Gui_SpinButton_Sample_Grid_Scale_Y, "value-changed", G_CALLBACK(value_changed_sample_grid_scale_y), self);
        g_signal_connect(Gui_ChainButton_Sample_Grid_Scale, "toggled", G_CALLBACK(value_changed_chain_button_sample_grid_scale), self);
        g_signal_connect(Gui_SpinButton_Sample_Weighting, "value-changed", G_CALLBACK(value_changed_sample_weighting), self);
        g_signal_connect(Gui_SpinButton_Sample_Count, "value-changed", G_CALLBACK(value_changed_sample_count_adj), self);
        g_signal_connect(Gui_Sample_Interpolation_X_SpinButton, "value-changed", G_CALLBACK(value_changed_interpolation_x), self);
        g_signal_connect(Gui_Sample_Interpolation_Y_SpinButton, "value-changed", G_CALLBACK(value_changed_interpolation_y), self);
        g_signal_connect(Gui_Sample_Interpolation_ChainButton, "toggled", G_CALLBACK(value_changed_chain_button_sample_interpolation), self);
        g_signal_connect(Gui_Chunk_Size_SpinButton, "value-changed", G_CALLBACK(value_changed_chunk_kilopixels), self);

        g_object_set_data(G_OBJECT(Gui_Button_Size_Reset), "button-name", button_pointer_Size_Reset);
        g_object_set_data(G_OBJECT(Gui_Button_flip_xy), "button-name", button_pointer_flip_xy);
        g_object_set_data(G_OBJECT(Gui_Button_m2), "button-name", button_pointer_m2);
        g_object_set_data(G_OBJECT(Gui_Button_m4), "button-name", button_pointer_m4);
        g_object_set_data(G_OBJECT(Gui_Button_m5), "button-name", button_pointer_m5);
        g_object_set_data(G_OBJECT(Gui_Button_m10), "button-name", button_pointer_m10);
        g_object_set_data(G_OBJECT(Gui_Button_d2), "button-name", button_pointer_d2);
        g_object_set_data(G_OBJECT(Gui_Button_d4), "button-name", button_pointer_d4);
        g_object_set_data(G_OBJECT(Gui_Button_d5), "button-name", button_pointer_d5);
        g_object_set_data(G_OBJECT(Gui_Button_d10), "button-name", button_pointer_d10);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_Reset), "button-name", button_pointer_sample_grid_scale_Reset);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_125), "button-name", button_pointer_sample_grid_scale_125);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_150), "button-name", button_pointer_sample_grid_scale_150);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_200), "button-name", button_pointer_sample_grid_scale_200);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_250), "button-name", button_pointer_sample_grid_scale_250);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_325), "button-name", button_pointer_sample_grid_scale_325);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_500), "button-name", button_pointer_sample_grid_scale_500);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_1200), "button-name", button_pointer_sample_grid_scale_1200);
        g_object_set_data(G_OBJECT(Gui_Button_Sample_Lerp_Quickset_Nearest), "button-name", button_pointer_sample_interpolation_nearest);
        g_object_set_data(G_OBJECT(Gui_Button_Sample_Lerp_Quickset_Linear), "button-name", button_pointer_sample_interpolation_linear);
        g_object_set_data(G_OBJECT(Gui_Button_Sample_Lerp_Quickset_Quadratic), "button-name", button_pointer_sample_interpolation_quadratic);
        g_object_set_data(G_OBJECT(Gui_Button_Sample_Lerp_Quickset_Cubic), "button-name", button_pointer_sample_interpolation_cubic);
        g_object_set_data(G_OBJECT(Gui_Button_Sample_Lerp_Quickset_Quartic), "button-name", button_pointer_sample_interpolation_quartic);
        g_object_set_data(G_OBJECT(Gui_Button_Sample_Lerp_Quickset_Quintic), "button-name", button_pointer_sample_interpolation_quintic);
        g_object_set_data(G_OBJECT(Gui_Button_Grid_Shape_Auto), "button-name", button_pointer_sample_grid_shape_auto);
        g_object_set_data(G_OBJECT(Gui_Button_Grid_Shape_Square), "button-name", button_pointer_sample_grid_shape_square);
        g_object_set_data(G_OBJECT(Gui_Button_Grid_Shape_Circle), "button-name", button_pointer_sample_grid_shape_circle);
        g_object_set_data(G_OBJECT(Gui_Button_Seamless_X), "button-name", button_pointer_seamless_x);
        g_object_set_data(G_OBJECT(Gui_Button_Seamless_Y), "button-name", button_pointer_seamless_y);
        g_object_set_data(G_OBJECT(Gui_Button_Plugin_Run), "button-name", button_pointer_run_plugin);
        g_object_set_data(G_OBJECT(Gui_Button_Plugin_Cancel), "button-name", button_pointer_cancel_plugin);
        g_object_set_data(G_OBJECT(Gui_Button_Plugin_Reset), "button-name", button_pointer_reset_plugin);

        g_signal_connect(Gui_Button_Size_Reset, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_flip_xy, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_m2, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_m4, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_m5, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_m10, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_d2, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_d4, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_d5, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_d10, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_Reset, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_125, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_150, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_200, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_250, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_325, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_500, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_1200, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Sample_Lerp_Quickset_Nearest, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Sample_Lerp_Quickset_Linear, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Sample_Lerp_Quickset_Quadratic, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Sample_Lerp_Quickset_Cubic, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Sample_Lerp_Quickset_Quartic, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Sample_Lerp_Quickset_Quintic, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Grid_Shape_Auto, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Grid_Shape_Square, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Grid_Shape_Circle, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Seamless_X, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Seamless_Y, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Plugin_Run, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Plugin_Cancel, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Plugin_Reset, "clicked", G_CALLBACK(value_changed_button), self);

        Params->CalcAll();
        SyncDataFromParameters();

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    ComboSizeWidget::~ComboSizeWidget()
    {
        delete button_pointer_Size_Reset;
        delete button_pointer_flip_xy;
        delete button_pointer_m2;
        delete button_pointer_m4;
        delete button_pointer_m5;
        delete button_pointer_m10;
        delete button_pointer_d2;
        delete button_pointer_d4;
        delete button_pointer_d5;
        delete button_pointer_d10;
        delete button_pointer_sample_grid_scale_Reset;
        delete button_pointer_sample_grid_scale_125;
        delete button_pointer_sample_grid_scale_150;
        delete button_pointer_sample_grid_scale_200;
        delete button_pointer_sample_grid_scale_250;
        delete button_pointer_sample_grid_scale_325;
        delete button_pointer_sample_grid_scale_500;
        delete button_pointer_sample_grid_scale_1200;
        delete button_pointer_sample_interpolation_nearest;
        delete button_pointer_sample_interpolation_linear;
        delete button_pointer_sample_interpolation_quadratic;
        delete button_pointer_sample_interpolation_cubic;
        delete button_pointer_sample_interpolation_quartic;
        delete button_pointer_sample_interpolation_quintic;
        delete button_pointer_sample_grid_shape_auto;
        delete button_pointer_sample_grid_shape_square;
        delete button_pointer_sample_grid_shape_circle;
        delete button_pointer_seamless_x;
        delete button_pointer_seamless_y;
        delete button_pointer_run_plugin;
        delete button_pointer_cancel_plugin;
        delete button_pointer_reset_plugin;

        self = NULL;
    }

    void ComboSizeWidget::SetSizeX(gint val)
    {
        if (ignore_auto_changes == true) return;

        constexpr f128 V_MAX = 65537._q;
        constexpr f128 V_MIN = 1._q / V_MAX;

        size_x = (gint) max((gint)val, (gint)1);
        f128 scale_x_f = f128(size_x) / f128(original_x);
        scale_x_f = clampq(scale_x_f, V_MIN, V_MAX);
        scale_x_f = rounddgq(scale_x_f, -5._q);
        scale_x = gdouble(scale_x_f);

        if (chain_button_on == TRUE)
        {
            size_y = (gint) max(gint(f128(size_x) / locked_ratio_xy), (gint)1);
            f128 n_scale = clampq(f128(size_y) / f128(original_y), V_MIN, V_MAX);
            scale_y = (gdouble)rounddgq(n_scale, -5._q);
        } else
        {
            locked_ratio_xy = SetRatio(size_x, size_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetSizeY(gint val)
    {
        if (ignore_auto_changes == true) return;

        constexpr f128 V_MAX = 65537._q;
        constexpr f128 V_MIN = 1._q / V_MAX;

        size_y = (gint) max((gint)val, (gint)1);
        f128 scale_y_f = f128(size_y) / f128(original_y);
        scale_y_f = clampq(scale_y_f, V_MIN, V_MAX);
        scale_y_f = rounddgq(scale_y_f, -5._q);
        scale_y = gdouble(scale_y_f);

        if (chain_button_on == TRUE)
        {
            size_x = (gint) max(gint(f128(size_y) * locked_ratio_xy), (gint)1);
            f128 n_scale = clampq(f128(size_x) / f128(original_x), V_MIN, V_MAX);
            scale_x = (gdouble)rounddgq(n_scale, -5._q);
        } else
        {
            locked_ratio_xy = SetRatio(size_x, size_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetScaleX(gdouble val)
    {
        if (ignore_auto_changes == true) return;

        constexpr f128 V_MAX = 65537._q;
        constexpr f128 V_MIN = 1._q / V_MAX;

        f128 value = clampq((f128)val * 0.01_q, V_MIN, V_MAX);
        value = rounddgq(value, -6._q);
        scale_x = gdouble(value);
        f128 size_x_f = f128(original_x) * value;
        size_x = (gint) max((gint)size_x_f, (gint)1);

        if (chain_button_on == TRUE)
        {
            size_y = (gint) max(gint(f128(size_x) / locked_ratio_xy), (gint)1);
            f128 n_scale = clampq(f128(size_y) / f128(original_y), V_MIN, V_MAX);
            scale_y = (gdouble)rounddgq(n_scale, -6._q);
        } else
        {
            locked_ratio_xy = SetRatio(size_x, size_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetScaleY(gdouble val)
    {
        if (ignore_auto_changes == true) return;

        constexpr f128 V_MAX = 65537._q;
        constexpr f128 V_MIN = 1._q / V_MAX;

        f128 value = clampq((f128)val * 0.01_q, V_MIN, V_MAX);
        value = rounddgq(value, -6._q);
        scale_y = gdouble(value);
        f128 size_y_f = f128(original_y) * value;
        size_y = (gint) max((gint)size_y_f, (gint)1);

        if (chain_button_on == TRUE)
        {
            size_x = (gint) max(gint(f128(size_y) * locked_ratio_xy), (gint)1);
            f128 n_scale = clampq(f128(size_x) / f128(original_x), V_MIN, V_MAX);
            scale_x = (gdouble)rounddgq(n_scale, -6._q);
        } else
        {
            locked_ratio_xy = SetRatio(size_x, size_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetScale_SampleGridScaleX(gdouble val)
    {
        if (ignore_auto_changes == true) return;

        f128 value = clampq((f128)val * 0.01_q, 0.01_q, 5000._q);
        value = rounddgq(value, -4._q);
        sample_grid_scale_x = gdouble(value);

        if (chain_button_sample_grid_scale_on == TRUE)
        {
            f128 sample_grid_scale_f = clampq(f128(sample_grid_scale_x) / f128(sample_grid_scale_locked_ratio_xy), 0._q, 5000._q);
            sample_grid_scale_y = (gdouble)rounddgq(sample_grid_scale_f, -4._q);
        } else
        {
            sample_grid_scale_locked_ratio_xy = SetRatio(sample_grid_scale_x, sample_grid_scale_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetScale_SampleGridScaleY(gdouble val)
    {
        if (ignore_auto_changes == true) return;

        f128 value = clampq((f128)val * 0.01_q, 0.01_q, 5000._q);
        value = rounddgq(value, -4._q);
        sample_grid_scale_y = gdouble(value);

        if (chain_button_sample_grid_scale_on == TRUE)
        {
            f128 sample_grid_scale_f = clampq(f128(sample_grid_scale_y) * f128(sample_grid_scale_locked_ratio_xy), 0._q, 5000._q);
            sample_grid_scale_x = (gdouble)rounddgq(sample_grid_scale_f, -4._q);
        } else
        {
            sample_grid_scale_locked_ratio_xy = SetRatio(sample_grid_scale_x, sample_grid_scale_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetSampleGridWeighting(gdouble val)
    {
        sample_grid_weighting = clampq((f128)val, 0.0_q, 1.0_q);
        sample_grid_weighting = rounddgq(sample_grid_weighting, -5._q);

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetSampleCountAdj(gdouble val)
    {
        sample_count_adjustment = clampq((f128)val * 0.01_q, 1.0_q, 16.0_q);
        sample_count_adjustment = rounddgq(sample_count_adjustment, -5._q);

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetSampleInterpolationX(gdouble val)
    {
        if (ignore_auto_changes == true) return;

        f128 value = clampq((f128)val, 0._q, 50._q);
        value = rounddgq(value, -2._q);
        sample_interpolation_x = gdouble(value);

        if (chain_button_sample_interpolation_on == TRUE)
        {
            f128 sample_interpolation_f = clampq(f128(sample_interpolation_x) / f128(sample_interpolation_locked_ratio_xy), 0._q, 50._q);
            sample_interpolation_y = (gdouble)rounddgq(sample_interpolation_f, -3._q);
        } else
        {
            sample_interpolation_locked_ratio_xy = SetRatio(sample_interpolation_x, sample_interpolation_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetSampleInterpolationY(gdouble val)
    {
        if (ignore_auto_changes == true) return;

        f128 value = clampq((f128)val, 0._q, 50._q);
        value = rounddgq(value, -2._q);
        sample_interpolation_y = gdouble(value);

        if (chain_button_sample_interpolation_on == TRUE)
        {
            f128 sample_interpolation_f = clampq(f128(sample_interpolation_y) * f128(sample_interpolation_locked_ratio_xy), 0._q, 50._q);
            sample_interpolation_x = (gdouble)rounddgq(sample_interpolation_f, -3._q);
        } else
        {
            sample_interpolation_locked_ratio_xy = SetRatio(sample_interpolation_x, sample_interpolation_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetKilopixels(gint kpx)
    {
        chunk_size_kilo = u64 (kpx);

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetChainButton(gboolean val)
    {
        if (ignore_auto_changes == true) return;

        if ((chain_button_on == TRUE) && (val == FALSE))
            locked_ratio_xy = SetRatio(size_x, size_y);

        chain_button_on = (gboolean) val;

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetChainButton_SampleGridScale(gboolean val)
    {
        if (ignore_auto_changes == true) return;

        if ((chain_button_sample_grid_scale_on == TRUE) && (val == FALSE))
            sample_grid_scale_locked_ratio_xy = SetRatio(sample_grid_scale_x, sample_grid_scale_y);

        chain_button_sample_grid_scale_on = (gboolean) val;

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetChainButton_SampleInterpolation(gboolean val)
    {
        if (ignore_auto_changes == true) return;

        if ((chain_button_sample_interpolation_on == TRUE) && (val == FALSE))
            sample_interpolation_locked_ratio_xy = SetRatio(sample_interpolation_x, sample_interpolation_y);

        chain_button_sample_interpolation_on = (gboolean) val;

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::ToggleChainButton()
    {
        if (ignore_auto_changes == true) return;

        if (chain_button_on == TRUE)
        {
            chain_button_on = (gboolean) FALSE;
        } else
        {
            chain_button_on = (gboolean) TRUE;
            locked_ratio_xy = SetRatio(size_x, size_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetOriginalSize(gint val_x, gint val_y)
    {
        original_x = (gint) max((gint)val_x, (gint)1);
        original_y = (gint) max((gint)val_y, (gint)1);

        size_x = original_x;
        size_y = original_y;
        scale_x = (gdouble) 1.0;
        scale_y = (gdouble) 1.0;
        original_ratio_xy = SetRatio(original_x, original_y);
        locked_ratio_xy = SetRatio(size_x, size_y);

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }

    void ComboSizeWidget::UpdateGUI()
    {
        if (Gui_Text_Size_Original_X)
            gtk_label_set_text((GtkLabel*)Gui_Text_Size_Original_X, g_strdup_printf(_("Width: %i" "%s"), original_x, "\n") );
        if (Gui_Text_Size_Original_Y)
            gtk_label_set_text((GtkLabel*)Gui_Text_Size_Original_Y, g_strdup_printf(_("Height: %i" "%s"), original_y, "\n") );

        if (Gui_SpinButton_Size_X_Pixels)
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Size_X_Pixels, (gdouble)size_x);
        if (Gui_SpinButton_Size_Y_Pixels)
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Size_Y_Pixels, (gdouble)size_y);
        if (Gui_SpinButton_Size_X_Scale)
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Size_X_Scale, (gdouble)scale_x * (gdouble)100.0);
        if (Gui_SpinButton_Size_Y_Scale)
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Size_Y_Scale, (gdouble)scale_y * (gdouble)100.0);
        if (Gui_ChainButton)
            gimp_chain_button_set_active((GimpChainButton*)Gui_ChainButton, chain_button_on);

        if (Gui_SpinButton_Sample_Grid_Scale_X)
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Sample_Grid_Scale_X, (gdouble)sample_grid_scale_x * (gdouble)100.0);
        if (Gui_SpinButton_Sample_Grid_Scale_Y)
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Sample_Grid_Scale_Y, (gdouble)sample_grid_scale_y * (gdouble)100.0);
        if (Gui_ChainButton_Sample_Grid_Scale)
            gimp_chain_button_set_active((GimpChainButton*)Gui_ChainButton_Sample_Grid_Scale, chain_button_sample_grid_scale_on);

        if (Gui_Sample_Interpolation_X_Label)
        {
            sample_interpolation_x_text = SetInterpolationString(sample_interpolation_x);
            gtk_label_set_text((GtkLabel*)Gui_Sample_Interpolation_X_Label,
                g_strdup_printf(_("Sample Interpolation X" "\n %s"), sample_interpolation_x_text.c_str()) );
        }
        if (Gui_Sample_Interpolation_Y_Label)
        {
            sample_interpolation_y_text = SetInterpolationString(sample_interpolation_y);
            gtk_label_set_text((GtkLabel*)Gui_Sample_Interpolation_Y_Label,
                g_strdup_printf(_("Sample Interpolation Y" "\n %s"), sample_interpolation_y_text.c_str()) );
        }
        if (Gui_Sample_Interpolation_X_SpinButton)
            gtk_spin_button_set_value((GtkSpinButton*)Gui_Sample_Interpolation_X_SpinButton, (gdouble)sample_interpolation_x);
        if (Gui_Sample_Interpolation_Y_SpinButton)
            gtk_spin_button_set_value((GtkSpinButton*)Gui_Sample_Interpolation_Y_SpinButton, (gdouble)sample_interpolation_y);
        if (Gui_Sample_Interpolation_ChainButton)
            gimp_chain_button_set_active((GimpChainButton*)Gui_Sample_Interpolation_ChainButton, chain_button_sample_interpolation_on);

        if (Gui_Sample_Grid_Shape_Label)
        {
            if (sample_grid_shape == SAMPLE_GRID_SHAPE_Auto)
                gtk_label_set_text((GtkLabel*)Gui_Sample_Grid_Shape_Label, g_strdup_printf(_("Sample Grid Shape:\n%s"), " Auto"));
            else if (sample_grid_shape == SAMPLE_GRID_SHAPE_Square)
                gtk_label_set_text((GtkLabel*)Gui_Sample_Grid_Shape_Label, g_strdup_printf(_("Sample Grid Shape:\n%s"), " Square"));
            else if (sample_grid_shape == SAMPLE_GRID_SHAPE_Circle)
                gtk_label_set_text((GtkLabel*)Gui_Sample_Grid_Shape_Label, g_strdup_printf(_("Sample Grid Shape:\n%s"), " Circle"));
        }

        if (Gui_Sample_Weighting_Label)
        {
            f128 rounded_weighting = rounddgq(sample_grid_weighting, -3._q);
            if (rounded_weighting < 0.000005_q)
                gtk_label_set_text((GtkLabel*)Gui_Sample_Weighting_Label,
                    g_strdup_printf(_("Sample Weighting\n" "%s"), " Off") );
            else if (rounded_weighting > 0.999995_q)
                gtk_label_set_text((GtkLabel*)Gui_Sample_Weighting_Label,
                    g_strdup_printf(_("Sample Weighting\n" "%s"), " Max") );
            else
                gtk_label_set_text((GtkLabel*)Gui_Sample_Weighting_Label,
                    g_strdup_printf(_("Sample Weighting\n" "%s"), " Partial") );
        }
        if (Gui_SpinButton_Sample_Weighting)
        {
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Sample_Weighting,
                (gdouble) sample_grid_weighting);
        }

        if (Gui_SpinButton_Sample_Count)
        {
            gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Sample_Count,
                gdouble (sample_count_adjustment * 100._q));
        }

        if (Gui_Button_Seamless_X)
        {
            if (seamless_x == TRUE)
                gtk_button_set_label((GtkButton*)Gui_Button_Seamless_X, "Seamless Looping Horizontal Borders");
            else
                gtk_button_set_label((GtkButton*)Gui_Button_Seamless_X, "Regular Horizontal Borders");
        }
        if (Gui_Button_Seamless_Y)
        {
            if (seamless_y == TRUE)
                gtk_button_set_label((GtkButton*)Gui_Button_Seamless_Y, "Seamless Looping Vertical Borders");
            else
                gtk_button_set_label((GtkButton*)Gui_Button_Seamless_Y, "Regular Vertical Borders");
        }

        if (Gui_Chunk_Size_Label)
        {
            gtk_label_set_text((GtkLabel*)Gui_Chunk_Size_Label, g_strdup_printf(_("Chunk Size, Kilosamples\n" "%I64u Ksmp, %I64u smp"), chunk_size_kilo, chunk_size_kilo * 1024uL) );
        }
        if (Gui_Chunk_Size_SpinButton)
        {
            gtk_spin_button_set_value((GtkSpinButton*)Gui_Chunk_Size_SpinButton, gdouble (chunk_size_kilo));
        }

        SyncDataToParameters();

        if (Log && Params)
        {
            Log->Log(false, Params->info_string);
        }
    }
    void ComboSizeWidget::SyncDataToParameters()
    {
        if (Params)
        {
            Params->input_size_x = (u64)original_x;
            Params->input_size_y = (u64)original_y;
            Params->output_size_x = (u64)size_x;
            Params->output_size_y = (u64)size_y;
            Params->seamless_x = (seamless_x == TRUE) ? true : false;
            Params->seamless_y = (seamless_y == TRUE) ? true : false;
            Params->sample_grid_width_percent = f64(sample_grid_scale_x * 100.0);
            Params->sample_grid_height_percent = f64(sample_grid_scale_y * 100.0);
            Params->sample_interpolation_x = (f128)sample_interpolation_x;
            Params->sample_interpolation_y = (f128)sample_interpolation_y;
            Params->sample_grid_shape = sample_grid_shape;
            Params->sample_grid_weighting = sample_grid_weighting;
            Params->sample_count_adjustment = sample_count_adjustment;
            Params->chunk_size_kilo = chunk_size_kilo;
            Params->CalcAll();
        }
    }
    void ComboSizeWidget::SyncDataFromParameters()
    {
        if (Params)
        {
            ignore_auto_changes = true;

            original_x = (gint)Params->input_size_x;
            original_y = (gint)Params->input_size_y;
            size_x = (gint) Params->output_size_x;
            size_y = (gint) Params->output_size_y;
            f128 scl_x = f128(size_x) / f128(original_x);
            f128 scl_y = f128(size_y) / f128(original_y);
            scale_x = (gdouble)scl_x;
            scale_y = (gdouble)scl_y;
            sample_grid_scale_x = (gdouble) Params->sample_grid_width_percent * (gdouble)0.01;
            sample_grid_scale_y = (gdouble) Params->sample_grid_height_percent * (gdouble)0.01;
            sample_interpolation_x = (gdouble) Params->sample_interpolation_x;
            sample_interpolation_y = (gdouble) Params->sample_interpolation_y;
            sample_grid_shape = Params->sample_grid_shape;
            sample_grid_weighting = Params->sample_grid_weighting;
            sample_count_adjustment = Params->sample_count_adjustment;
            seamless_x = (Params->seamless_x == true) ? TRUE : FALSE;
            seamless_y = (Params->seamless_y == true) ? TRUE : FALSE;
            chunk_size_kilo = Params->chunk_size_kilo;

            original_ratio_xy = SetRatio(original_x, original_y);
            locked_ratio_xy = SetRatio(size_x, size_y);
            sample_grid_scale_locked_ratio_xy = SetRatio(sample_grid_scale_x, sample_grid_scale_y);
            sample_interpolation_locked_ratio_xy = SetRatio(sample_interpolation_x, sample_interpolation_y);

            UpdateGUI();

            ignore_auto_changes = false;
        }
    }
    void ComboSizeWidget::SetButtonClicked(u16* button_name)
    {
        if (button_name == button_pointer_Size_Reset)
        {
            size_x = (gint) original_x;
            size_y = (gint) original_y;
            scale_x = (gdouble) 1.0;
            scale_y = (gdouble) 1.0;
            locked_ratio_xy = SetRatio(size_x, size_y);

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_m2)
        {
            size_x *= 2;
            size_y *= 2;
            scale_x = f64((f128)size_x / (f128)original_x);
            scale_y = f64((f128)size_y / (f128)original_y);
            if (chain_button_on == FALSE)
                locked_ratio_xy = SetRatio(size_x, size_y);

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_m4)
        {
            size_x *= 4;
            size_y *= 4;
            scale_x = f64((f128)size_x / (f128)original_x);
            scale_y = f64((f128)size_y / (f128)original_y);
            if (chain_button_on == FALSE)
                locked_ratio_xy = SetRatio(size_x, size_y);

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_m5)
        {
            size_x *= 5;
            size_y *= 5;
            scale_x = f64((f128)size_x / (f128)original_x);
            scale_y = f64((f128)size_y / (f128)original_y);
            if (chain_button_on == FALSE)
                locked_ratio_xy = SetRatio(size_x, size_y);

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_m10)
        {
            size_x *= 10;
            size_y *= 10;
            scale_x = f64((f128)size_x / (f128)original_x);
            scale_y = f64((f128)size_y / (f128)original_y);
            if (chain_button_on == FALSE)
                locked_ratio_xy = SetRatio(size_x, size_y);

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_d2)
        {
            size_x /= 2;
            size_y /= 2;
            scale_x = f64((f128)size_x / (f128)original_x);
            scale_y = f64((f128)size_y / (f128)original_y);
            if (chain_button_on == FALSE)
                locked_ratio_xy = SetRatio(size_x, size_y);

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_d4)
        {
            size_x /= 4;
            size_y /= 4;
            scale_x = f64((f128)size_x / (f128)original_x);
            scale_y = f64((f128)size_y / (f128)original_y);
            if (chain_button_on == FALSE)
                locked_ratio_xy = SetRatio(size_x, size_y);

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_d5)
        {
            size_x /= 5;
            size_y /= 5;
            scale_x = f64((f128)size_x / (f128)original_x);
            scale_y = f64((f128)size_y / (f128)original_y);
            if (chain_button_on == FALSE)
                locked_ratio_xy = SetRatio(size_x, size_y);

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_d10)
        {
            size_x /= 10;
            size_y /= 10;
            scale_x = f64((f128)size_x / (f128)original_x);
            scale_y = f64((f128)size_y / (f128)original_y);
            if (chain_button_on == FALSE)
                locked_ratio_xy = SetRatio(size_x, size_y);

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_flip_xy)
        {
            gint old_size_x = size_x;
            gint old_size_y = size_y;
            size_x = old_size_y;
            size_y = old_size_x;
            scale_x = f64((f128)size_x / (f128)original_x);
            scale_y = f64((f128)size_y / (f128)original_y);
            locked_ratio_xy = f128(size_x) / f128(size_y);

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_grid_scale_Reset)
        {
            ignore_auto_changes = true;

            sample_grid_scale_x = (gdouble) 1.0;
            sample_grid_scale_y = (gdouble) 1.0;
            sample_grid_scale_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_grid_scale_125)
        {
            ignore_auto_changes = true;

            sample_grid_scale_x = (gdouble) 1.25;
            sample_grid_scale_y = (gdouble) 1.25;
            sample_grid_scale_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_grid_scale_150)
        {
            ignore_auto_changes = true;

            sample_grid_scale_x = (gdouble) 1.50;
            sample_grid_scale_y = (gdouble) 1.50;
            sample_grid_scale_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_grid_scale_200)
        {
            ignore_auto_changes = true;

            sample_grid_scale_x = (gdouble) 2.0;
            sample_grid_scale_y = (gdouble) 2.0;
            sample_grid_scale_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_grid_scale_250)
        {
            ignore_auto_changes = true;

            sample_grid_scale_x = (gdouble) 2.50;
            sample_grid_scale_y = (gdouble) 2.50;
            sample_grid_scale_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_grid_scale_325)
        {
            ignore_auto_changes = true;

            sample_grid_scale_x = (gdouble) 3.25;
            sample_grid_scale_y = (gdouble) 3.25;
            sample_grid_scale_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_grid_scale_500)
        {
            ignore_auto_changes = true;

            sample_grid_scale_x = (gdouble) 5.0;
            sample_grid_scale_y = (gdouble) 5.0;
            sample_grid_scale_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_grid_scale_1200)
        {
            ignore_auto_changes = true;

            sample_grid_scale_x = (gdouble) 12.0;
            sample_grid_scale_y = (gdouble) 12.0;
            sample_grid_scale_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_grid_shape_auto)
        {
            ignore_auto_changes = true;

            sample_grid_shape = SAMPLE_GRID_SHAPE_Auto;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_grid_shape_square)
        {
            ignore_auto_changes = true;

            sample_grid_shape = SAMPLE_GRID_SHAPE_Square;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_grid_shape_circle)
        {
            ignore_auto_changes = true;

            sample_grid_shape = SAMPLE_GRID_SHAPE_Circle;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_interpolation_nearest)
        {
            ignore_auto_changes = true;

            sample_interpolation_x = (gdouble) 0.0;
            sample_interpolation_y = (gdouble) 0.0;
            sample_interpolation_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_interpolation_linear)
        {
            ignore_auto_changes = true;

            sample_interpolation_x = (gdouble) 1.0;
            sample_interpolation_y = (gdouble) 1.0;
            sample_interpolation_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_interpolation_quadratic)
        {
            ignore_auto_changes = true;

            sample_interpolation_x = (gdouble) 2.0;
            sample_interpolation_y = (gdouble) 2.0;
            sample_interpolation_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_interpolation_cubic)
        {
            ignore_auto_changes = true;

            sample_interpolation_x = (gdouble) 3.0;
            sample_interpolation_y = (gdouble) 3.0;
            sample_interpolation_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_interpolation_quartic)
        {
            ignore_auto_changes = true;

            sample_interpolation_x = (gdouble) 4.0;
            sample_interpolation_y = (gdouble) 4.0;
            sample_interpolation_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_sample_interpolation_quintic)
        {
            ignore_auto_changes = true;

            sample_interpolation_x = (gdouble) 5.0;
            sample_interpolation_y = (gdouble) 5.0;
            sample_interpolation_locked_ratio_xy = 1.0_q;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_seamless_x)
        {
            ignore_auto_changes = true;

            gboolean seamless = (seamless_x == TRUE) ? FALSE : TRUE;
            seamless_x = seamless;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_seamless_y)
        {
            ignore_auto_changes = true;

            gboolean seamless = (seamless_y == TRUE) ? FALSE : TRUE;
            seamless_y = seamless;

            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_cancel_plugin)
        {
            choices_done_result = -1;
        }
        else if (button_name == button_pointer_run_plugin)
        {
            choices_done_result = 1;

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_reset_plugin)
        {
            Params->Reset();
            SyncDataFromParameters();

            chain_button_on = TRUE;
            chain_button_sample_grid_scale_on = TRUE;
            chain_button_sample_interpolation_on = TRUE;

            SetButtonClicked(button_pointer_Size_Reset);
            SetButtonClicked(button_pointer_sample_grid_scale_Reset);
        }
    }
    s8 ComboSizeWidget::GetChoicesDoneResult()
    {
        return choices_done_result;
    }
    f128 ComboSizeWidget::SetRatio(gint vx, gint vy)
    {
        constexpr f128 V_MAX = 65536._q * 65536._q * 2._q;
        constexpr f128 V_MIN = 1._q / V_MAX;

        f128 v = clampq((f128)vx, V_MIN, V_MAX) / clampq((f128)vy, V_MIN, V_MAX);
        return clampq(v, V_MIN, V_MAX);
    }
    f128 ComboSizeWidget::SetRatio(f64 vx, f64 vy)
    {
        constexpr f128 V_MAX = 65536._q * 65536._q * 2._q;
        constexpr f128 V_MIN = 1._q / V_MAX;

        f128 v = clampq((f128)vx, V_MIN, V_MAX) / clampq((f128)vy, V_MIN, V_MAX);
        return clampq(v, V_MIN, V_MAX);
    }
    f128 ComboSizeWidget::SetRatio(f128 vx, f128 vy)
    {
        constexpr f128 V_MAX = 65536._q * 65536._q * 2._q;
        constexpr f128 V_MIN = 1._q / V_MAX;

        f128 v = clampq(vx, V_MIN, V_MAX) / clampq(vy, V_MIN, V_MAX);
        return clampq(v, V_MIN, V_MAX);
    }
    string ComboSizeWidget::SetInterpolationString(f64 v)
    {
        f64 rv = rounddg(v, -5.0);

        if (rv < 0.000005) return "Nearest";
        if (rv < 1.000005) return "Linear";
        if (rv < 2.00000) return "Linear to Quadratic";
        if (rv < 2.000005) return "Quadratic";
        if (rv < 3.00000) return "Quadratic to Cubic";
        if (rv < 3.000005) return "Cubic";
        if (rv < 4.00000) return "Cubic to Quartic";
        if (rv < 4.000005) return "Quartic";
        if (rv < 5.00000) return "Quartic to Quintic";
        if (rv < 5.000005) return "Quintic";
        return "Beyond Quintic";
    }

}; // END namespace TritonhawkPlus
