/*
Copyright (c) Tiger's Eye Jade Swiftwing, all rights reserved.

This file is written by Tiger's Eye Jade Swiftwing.  It is licensed under the GPLv3 license.
Note that my first name is "Tiger's Eye" (which is two words), my middle name is "Jade", and "Swiftwing" is one word that is my last name.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "components/thp_types.h"
#include "components/thp_gui-combowidget-size.h"
#include "components/thp_parameters.h"
#include "components/thp_log.h"

using namespace TritonhawkPlus;
using namespace std;
using namespace quadmath;

using namespace TritonhawkPlus;

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
    void value_changed_button(GimpButton* self, gpointer user_data)
    {
        int* name = (int*)g_object_get_data((GObject*)self, "button-name");
        ((ComboSizeWidget*)(user_data))->SetButtonClicked(name);
    };

    ComboSizeWidget::ComboSizeWidget(GtkWidget* dialog_in, ThpParams* params_in, ThpLog* log_in)
    {
        if (!dialog_in) return;
        dialog = dialog_in;
        if (params_in) Params = params_in;
        if (log_in) Log = log_in;

        gint master_width = (gint) 450;
        gint cell_height = (gint) 40;
        gint cell_height_half = (gint) 20;
        gint box_padding = (gint) 4;
        gint chain_button_width = (gint) 28;
        gint cell_quickset_button_width = (gint) 25;
        gint cell_r0_width = (gint) 176;
        gint row_0_height = 100;
        gint cell_r1_width = (gint) 45;
        gint row_1_height = 20;
        gint cell_r2_width = (gint) 150;
        gint row_2_height = cell_height * (gint) 2;
        gint sample_grid_quickset_buttons_width = (gint) 175;
        gint cell_r3_width = (gint) 300;
        gint row_3_height = cell_height;

        // Master Box
        Gui_Box_Master = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_Master), 0);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_Master), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_Master), GTK_ALIGN_START);
        gtk_widget_set_size_request(Gui_Box_Master, master_width, 160);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG (dialog))), Gui_Box_Master);
        gtk_widget_show(Gui_Box_Master);

        // First Row Box
        Gui_Box_H_Row_0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_0), box_padding);
        gtk_widget_set_size_request(Gui_Box_H_Row_0, master_width, row_0_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_H_Row_0), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_H_Row_0), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_0, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_H_Row_0);

        // First Row, First Column Box
        Gui_Box_V_Size_Original = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Size_Original), box_padding);
        gtk_widget_set_size_request(Gui_Box_V_Size_Original, cell_r0_width, row_0_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_V_Size_Original), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_V_Size_Original), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_0), (GtkWidget*)Gui_Box_V_Size_Original, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_V_Size_Original);
        // Text Label that marks the original image data's dimensions
        Gui_Text_Size_Original_Label = gtk_label_new(NULL);
        gtk_widget_set_size_request(Gui_Text_Size_Original_Label, cell_r0_width, cell_height_half);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Text_Size_Original_Label), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Text_Size_Original_Label), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Original), (GtkWidget*)Gui_Text_Size_Original_Label, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Text_Size_Original_Label);
        gtk_label_set_text((GtkLabel*)Gui_Text_Size_Original_Label, g_strdup_printf(_("%s" "Original Size"), "") );
        // Text Label that marks the original image data's dimensions
        Gui_Text_Size_Original_X = gtk_label_new(NULL);
        gtk_widget_set_size_request(Gui_Text_Size_Original_X, cell_r0_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Text_Size_Original_X), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Text_Size_Original_X), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Original), (GtkWidget*)Gui_Text_Size_Original_X, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Text_Size_Original_X);
        gtk_label_set_text((GtkLabel*)Gui_Text_Size_Original_X, g_strdup_printf(_("Width: %i" "%s"), original_x, "") );
        // Text Label that marks the original image data's dimensions
        Gui_Text_Size_Original_Y = gtk_label_new(NULL);
        gtk_widget_set_size_request(Gui_Text_Size_Original_Y, cell_r0_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Text_Size_Original_Y), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Text_Size_Original_Y), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Original), (GtkWidget*)Gui_Text_Size_Original_Y, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Text_Size_Original_Y);
        gtk_label_set_text((GtkLabel*)Gui_Text_Size_Original_Y, g_strdup_printf(_("Height: %i" "%s"), original_y, "") );

        // First Row, Second Column Box
        Gui_Box_V_Size_Pixels = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Size_Pixels), box_padding);
        gtk_widget_set_size_request(Gui_Box_V_Size_Pixels, cell_r0_width, row_0_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_V_Size_Pixels), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_V_Size_Pixels), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_0), (GtkWidget*)Gui_Box_V_Size_Pixels, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_V_Size_Pixels);
        // Text Label that marks the new image data's dimensions
        Gui_Text_Size_New_Pixels_Label = gtk_label_new(NULL);
        gtk_widget_set_size_request(Gui_Text_Size_New_Pixels_Label, cell_r0_width, cell_height_half);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Text_Size_New_Pixels_Label), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Text_Size_New_Pixels_Label), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Pixels), (GtkWidget*)Gui_Text_Size_New_Pixels_Label, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Text_Size_New_Pixels_Label);
        gtk_label_set_text((GtkLabel*)Gui_Text_Size_New_Pixels_Label, g_strdup_printf(_("%s" "New Size (Pixels)"), "") );
        // Spin Button that determines the new image data's dimensions
        Gui_SpinButton_Size_X_Pixels = gimp_spin_button_new_with_range((gdouble) 1.0, (gdouble) 65536.0, (gdouble) 1.0);
        gtk_widget_set_size_request(Gui_SpinButton_Size_X_Pixels, cell_r0_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_SpinButton_Size_X_Pixels), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_SpinButton_Size_X_Pixels), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Pixels), (GtkWidget*)Gui_SpinButton_Size_X_Pixels, FALSE, FALSE, 0);
        gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Size_X_Pixels, (guint) 0);
        gtk_widget_show(Gui_SpinButton_Size_X_Pixels);
        gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Size_X_Pixels, (gdouble)size_x);
        // Spin Button that determines the new image data's dimensions
        Gui_SpinButton_Size_Y_Pixels = gimp_spin_button_new_with_range((gdouble) 1.0, (gdouble) 65536.0, (gdouble) 1.0);
        gtk_widget_set_size_request(Gui_SpinButton_Size_Y_Pixels, cell_r0_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_SpinButton_Size_Y_Pixels), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_SpinButton_Size_Y_Pixels), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Pixels), (GtkWidget*)Gui_SpinButton_Size_Y_Pixels, FALSE, FALSE, 0);
        gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Size_Y_Pixels, (guint) 0);
        gtk_widget_show(Gui_SpinButton_Size_Y_Pixels);
        gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Size_Y_Pixels, (gdouble)size_y);

        // First Row, Third Column Box
        Gui_Box_V_Size_Scale = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Size_Scale), box_padding);
        gtk_widget_set_size_request(Gui_Box_V_Size_Scale, cell_r0_width, row_0_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_V_Size_Scale), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_V_Size_Scale), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_0), (GtkWidget*)Gui_Box_V_Size_Scale, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_V_Size_Scale);
        // Text Label that marks the new image data's scale
        Gui_Text_Size_New_Scale_Label = gtk_label_new(NULL);
        gtk_widget_set_size_request(Gui_Text_Size_New_Scale_Label, cell_r0_width, cell_height_half);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Text_Size_New_Scale_Label), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Text_Size_New_Scale_Label), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Scale), (GtkWidget*)Gui_Text_Size_New_Scale_Label, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Text_Size_New_Scale_Label);
        gtk_label_set_text((GtkLabel*)Gui_Text_Size_New_Scale_Label, g_strdup_printf(_("%s" "New Scale (Percent)"), "") );
        // Spin Button that determines the new image data's scale
        Gui_SpinButton_Size_X_Scale = gimp_spin_button_new_with_range((gdouble) 0.01, (gdouble) 99999.9999, (gdouble) 1.0);
        gtk_widget_set_size_request(Gui_SpinButton_Size_X_Scale, cell_r0_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_SpinButton_Size_X_Scale), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_SpinButton_Size_X_Scale), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Scale), (GtkWidget*)Gui_SpinButton_Size_X_Scale, FALSE, FALSE, 0);
        gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Size_X_Scale, (guint) 4);
        gtk_widget_show(Gui_SpinButton_Size_X_Scale);
        gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Size_X_Scale, (gdouble)scale_x * (gdouble)100.0);
        // Spin Button that determines the new image data's scale
        Gui_SpinButton_Size_Y_Scale = gimp_spin_button_new_with_range((gdouble) 0.01, (gdouble) 99999.9999, (gdouble) 1.0);
        gtk_widget_set_size_request(Gui_SpinButton_Size_Y_Scale, cell_r0_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_SpinButton_Size_Y_Scale), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_SpinButton_Size_Y_Scale), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Size_Scale), (GtkWidget*)Gui_SpinButton_Size_Y_Scale, FALSE, FALSE, 0);
        gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Size_Y_Scale, (guint) 4);
        gtk_widget_show(Gui_SpinButton_Size_Y_Scale);
        gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Size_Y_Scale, (gdouble)scale_y * (gdouble)100.0);

        // First Row, Fourth Column Box
        Gui_Box_V_Chainbutton = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Chainbutton), box_padding);
        gtk_widget_set_size_request(Gui_Box_V_Chainbutton, 26, row_0_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_V_Chainbutton), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_V_Chainbutton), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_0), (GtkWidget*)Gui_Box_V_Chainbutton, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_V_Chainbutton);
        // First Row, Fourth Column Box Spacer, empty space above chain button
        Gui_Box_V_Spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Spacer), 0);
        gtk_widget_set_size_request(Gui_Box_V_Spacer, 26, cell_height_half);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_V_Spacer), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_V_Spacer), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Chainbutton), (GtkWidget*)Gui_Box_V_Spacer, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_V_Spacer);
        // Chain button that links X and Y dimensions to optionally maintain ratio
        Gui_ChainButton = gimp_chain_button_new(GIMP_CHAIN_RIGHT);
        gtk_widget_set_size_request(Gui_ChainButton, chain_button_width, cell_height + cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_ChainButton), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_ChainButton), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Chainbutton), (GtkWidget*)Gui_ChainButton, FALSE, FALSE, 0);
        gtk_widget_show(Gui_ChainButton);

        // Row 1 Box
        Gui_Box_H_Row_1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_1), 0);
        gtk_widget_set_size_request(Gui_Box_H_Row_1, master_width, row_1_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_H_Row_1), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_H_Row_1), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_1, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_H_Row_1);

        // Button, Reset
        Gui_Button_Size_Reset = gtk_button_new_with_label("Reset");
        gtk_widget_set_size_request(Gui_Button_Size_Reset, cell_r1_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_Size_Reset), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_Size_Reset), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_Size_Reset, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_Size_Reset);
        // Button, Multiply by 2
        Gui_Button_m2 = gtk_button_new_with_label("x 2");
        gtk_widget_set_size_request(Gui_Button_m2, cell_r1_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_m2), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_m2), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_m2, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_m2);
        // Button, Multiply by 5
        Gui_Button_m5 = gtk_button_new_with_label("x 5");
        gtk_widget_set_size_request(Gui_Button_m5, cell_r1_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_m5), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_m5), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_m5, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_m5);
        // Button, Multiply by 10
        Gui_Button_m10 = gtk_button_new_with_label("x 10");
        gtk_widget_set_size_request(Gui_Button_m10, cell_r1_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_m10), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_m10), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_m10, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_m10);
        // Button, Divide by 2
        Gui_Button_d2 = gtk_button_new_with_label("x 1/2");
        gtk_widget_set_size_request(Gui_Button_d2, cell_r1_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_d2), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_d2), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_d2, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_d2);
        // Button, Divide by 5
        Gui_Button_d5 = gtk_button_new_with_label("x 1/5");
        gtk_widget_set_size_request(Gui_Button_d5, cell_r1_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_d5), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_d5), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_d5, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_d5);
        // Button, Divide by 10
        Gui_Button_d10 = gtk_button_new_with_label("x 1/10");
        gtk_widget_set_size_request(Gui_Button_d10, cell_r1_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_d10), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_d10), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_d10, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_d10);
        // Button, Swap X and Y sizes
        Gui_Button_flip_xy = gtk_button_new_with_label("Flip X-Y");
        gtk_widget_set_size_request(Gui_Button_flip_xy, cell_r1_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_flip_xy), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_flip_xy), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_1), (GtkWidget*)Gui_Button_flip_xy, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_flip_xy);

        // Row 2 Box
        Gui_Box_H_Row_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_2), box_padding);
        gtk_widget_set_size_request(Gui_Box_H_Row_2, master_width, row_2_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_H_Row_2), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_H_Row_2), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_2, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_H_Row_2);

        // Row 2, Column 0 Box
        Gui_Box_V_Sample_Grid_Scale_Labels = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Sample_Grid_Scale_Labels), box_padding);
        gtk_widget_set_size_request(Gui_Box_V_Sample_Grid_Scale_Labels, cell_r2_width, row_2_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_V_Sample_Grid_Scale_Labels), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_V_Sample_Grid_Scale_Labels), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_2), (GtkWidget*)Gui_Box_V_Sample_Grid_Scale_Labels, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_V_Sample_Grid_Scale_Labels);
        // Sample Grid Scale Label, X
        Gui_SpinButton_Sample_Grid_Scale_X_Label = gtk_label_new(NULL);
        gtk_widget_set_size_request(Gui_SpinButton_Sample_Grid_Scale_X_Label, cell_r2_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_SpinButton_Sample_Grid_Scale_X_Label), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_SpinButton_Sample_Grid_Scale_X_Label), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Grid_Scale_Labels), (GtkWidget*)Gui_SpinButton_Sample_Grid_Scale_X_Label, FALSE, FALSE, 0);
        gtk_widget_show(Gui_SpinButton_Sample_Grid_Scale_X_Label);
        gtk_label_set_text((GtkLabel*)Gui_SpinButton_Sample_Grid_Scale_X_Label, g_strdup_printf(_("Sample Grid Scale X (Percent)" "%s"), "") );
        // Sample Grid Scale Label, Y
        Gui_SpinButton_Sample_Grid_Scale_Y_Label = gtk_label_new(NULL);
        gtk_widget_set_size_request(Gui_SpinButton_Sample_Grid_Scale_Y_Label, cell_r2_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_SpinButton_Sample_Grid_Scale_Y_Label), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_SpinButton_Sample_Grid_Scale_Y_Label), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Grid_Scale_Labels), (GtkWidget*)Gui_SpinButton_Sample_Grid_Scale_Y_Label, FALSE, FALSE, 0);
        gtk_widget_show(Gui_SpinButton_Sample_Grid_Scale_Y_Label);
        gtk_label_set_text((GtkLabel*)Gui_SpinButton_Sample_Grid_Scale_Y_Label, g_strdup_printf(_("Sample Grid Scale Y (Percent)" "%s"), "") );

        // Row 2, Column 1 Box
        Gui_Box_V_Sample_Grid_Scale_SpinButtons = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Sample_Grid_Scale_SpinButtons), box_padding);
        gtk_widget_set_size_request(Gui_Box_V_Sample_Grid_Scale_SpinButtons, cell_r2_width, row_2_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_V_Sample_Grid_Scale_SpinButtons), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_V_Sample_Grid_Scale_SpinButtons), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_2), (GtkWidget*)Gui_Box_V_Sample_Grid_Scale_SpinButtons, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_V_Sample_Grid_Scale_SpinButtons);
        // Spin Button that determines the sample grid's scale, X
        Gui_SpinButton_Sample_Grid_Scale_X = gimp_spin_button_new_with_range((gdouble) 0.01, (gdouble) 3200.0, (gdouble) 1.00);
        gtk_widget_set_size_request(Gui_SpinButton_Sample_Grid_Scale_X, cell_r2_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_SpinButton_Sample_Grid_Scale_X), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_SpinButton_Sample_Grid_Scale_X), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Grid_Scale_SpinButtons), (GtkWidget*)Gui_SpinButton_Sample_Grid_Scale_X, FALSE, FALSE, 0);
        gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Sample_Grid_Scale_X, (guint) 2);
        gtk_widget_show(Gui_SpinButton_Sample_Grid_Scale_X);
        gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Sample_Grid_Scale_X, (gdouble)sample_grid_scale_x * (gdouble)100.0);
        // Spin Button that determines the sample grid's scale, Y
        Gui_SpinButton_Sample_Grid_Scale_Y = gimp_spin_button_new_with_range((gdouble) 0.01, (gdouble) 3200.0, (gdouble) 1.00);
        gtk_widget_set_size_request(Gui_SpinButton_Sample_Grid_Scale_Y, cell_r2_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_SpinButton_Sample_Grid_Scale_Y), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_SpinButton_Sample_Grid_Scale_Y), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Grid_Scale_SpinButtons), (GtkWidget*)Gui_SpinButton_Sample_Grid_Scale_Y, FALSE, FALSE, 0);
        gtk_spin_button_set_digits((GtkSpinButton*)Gui_SpinButton_Sample_Grid_Scale_Y, (guint) 2);
        gtk_widget_show(Gui_SpinButton_Sample_Grid_Scale_Y);
        gtk_spin_button_set_value((GtkSpinButton*)Gui_SpinButton_Sample_Grid_Scale_Y, (gdouble)sample_grid_scale_y * (gdouble)100.0);
        // Chain Button that determines when the sample grid's scale has it's aspect ratio locked
        Gui_ChainButton_Sample_Grid_Scale = gimp_chain_button_new(GIMP_CHAIN_RIGHT);
        gtk_widget_set_size_request(Gui_ChainButton_Sample_Grid_Scale, chain_button_width, cell_height * (gint)2);
        gtk_widget_set_halign(GTK_WIDGET(Gui_ChainButton_Sample_Grid_Scale), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_ChainButton_Sample_Grid_Scale), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_2), (GtkWidget*)Gui_ChainButton_Sample_Grid_Scale, FALSE, FALSE, 0);
        gtk_widget_show(Gui_ChainButton_Sample_Grid_Scale);

        // Sample Grid Scale Quickset Buttons
        Gui_Box_V_Sample_Grid_Scale_QuickSetButtons = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Sample_Grid_Scale_QuickSetButtons), box_padding);
        gtk_widget_set_size_request(Gui_Box_V_Sample_Grid_Scale_QuickSetButtons, sample_grid_quickset_buttons_width, row_2_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_V_Sample_Grid_Scale_QuickSetButtons), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_V_Sample_Grid_Scale_QuickSetButtons), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_2), (GtkWidget*)Gui_Box_V_Sample_Grid_Scale_QuickSetButtons, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_V_Sample_Grid_Scale_QuickSetButtons);
        // Sample Grid Scale Quickset Buttons, Row 0
        Gui_Box_H_Sample_Grid_Scale_QuickSet_0 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Sample_Grid_Scale_QuickSet_0), 0);
        gtk_widget_set_size_request(Gui_Box_H_Sample_Grid_Scale_QuickSet_0, sample_grid_quickset_buttons_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_H_Sample_Grid_Scale_QuickSet_0), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_H_Sample_Grid_Scale_QuickSet_0), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Grid_Scale_QuickSetButtons), (GtkWidget*)Gui_Box_H_Sample_Grid_Scale_QuickSet_0, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_H_Sample_Grid_Scale_QuickSet_0);
        // Sample Grid Scale Quickset Button
        Gui_Button_SampleGridScale_Reset = gtk_button_new_with_label("Reset");
        gtk_widget_set_size_request(Gui_Button_SampleGridScale_Reset, cell_quickset_button_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_SampleGridScale_Reset), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_SampleGridScale_Reset), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_0), (GtkWidget*)Gui_Button_SampleGridScale_Reset, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_SampleGridScale_Reset);
        // Sample Grid Scale Quickset Button
        Gui_Button_SampleGridScale_125 = gtk_button_new_with_label("%125");
        gtk_widget_set_size_request(Gui_Button_SampleGridScale_125, cell_quickset_button_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_SampleGridScale_125), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_SampleGridScale_125), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_0), (GtkWidget*)Gui_Button_SampleGridScale_125, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_SampleGridScale_125);
        // Sample Grid Scale Quickset Button
        Gui_Button_SampleGridScale_150 = gtk_button_new_with_label("%150");
        gtk_widget_set_size_request(Gui_Button_SampleGridScale_150, cell_quickset_button_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_SampleGridScale_150), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_SampleGridScale_150), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_0), (GtkWidget*)Gui_Button_SampleGridScale_150, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_SampleGridScale_150);
        // Sample Grid Scale Quickset Button
        Gui_Button_SampleGridScale_200 = gtk_button_new_with_label("%200");
        gtk_widget_set_size_request(Gui_Button_SampleGridScale_200, cell_quickset_button_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_SampleGridScale_200), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_SampleGridScale_200), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_0), (GtkWidget*)Gui_Button_SampleGridScale_200, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_SampleGridScale_200);
        // Sample Grid Scale Quickset Buttons, Row 1
        Gui_Box_H_Sample_Grid_Scale_QuickSet_1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Sample_Grid_Scale_QuickSet_1), 0);
        gtk_widget_set_size_request(Gui_Box_H_Sample_Grid_Scale_QuickSet_1, sample_grid_quickset_buttons_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_H_Sample_Grid_Scale_QuickSet_1), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_H_Sample_Grid_Scale_QuickSet_1), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Grid_Scale_QuickSetButtons), (GtkWidget*)Gui_Box_H_Sample_Grid_Scale_QuickSet_1, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_H_Sample_Grid_Scale_QuickSet_1);
        // Sample Grid Scale Quickset Button
        Gui_Button_SampleGridScale_250 = gtk_button_new_with_label("%250");
        gtk_widget_set_size_request(Gui_Button_SampleGridScale_250, cell_quickset_button_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_SampleGridScale_250), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_SampleGridScale_250), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_1), (GtkWidget*)Gui_Button_SampleGridScale_250, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_SampleGridScale_250);
        // Sample Grid Scale Quickset Button
        Gui_Button_SampleGridScale_325 = gtk_button_new_with_label("%325");
        gtk_widget_set_size_request(Gui_Button_SampleGridScale_325, cell_quickset_button_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_SampleGridScale_325), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_SampleGridScale_325), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_1), (GtkWidget*)Gui_Button_SampleGridScale_325, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_SampleGridScale_325);
        // Sample Grid Scale Quickset Button
        Gui_Button_SampleGridScale_500 = gtk_button_new_with_label("%500");
        gtk_widget_set_size_request(Gui_Button_SampleGridScale_500, cell_quickset_button_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_SampleGridScale_500), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_SampleGridScale_500), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_1), (GtkWidget*)Gui_Button_SampleGridScale_500, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_SampleGridScale_500);
        // Sample Grid Scale Quickset Button
        Gui_Button_SampleGridScale_1200 = gtk_button_new_with_label("%1200");
        gtk_widget_set_size_request(Gui_Button_SampleGridScale_1200, cell_quickset_button_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_SampleGridScale_1200), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_SampleGridScale_1200), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Sample_Grid_Scale_QuickSet_1), (GtkWidget*)Gui_Button_SampleGridScale_1200, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_SampleGridScale_1200);

        // Row 3 Box
        Gui_Box_H_Row_3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_3), box_padding);
        gtk_widget_set_size_request(Gui_Box_H_Row_3, master_width, row_2_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_H_Row_3), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_H_Row_3), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_3, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_H_Row_3);

        // Row 3, Column 0 Box
        Gui_Box_V_Sample_Interpolation_Labels = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Sample_Interpolation_Labels), box_padding);
        gtk_widget_set_size_request(Gui_Box_V_Sample_Interpolation_Labels, cell_r2_width, row_2_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_V_Sample_Interpolation_Labels), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_V_Sample_Interpolation_Labels), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_3), (GtkWidget*)Gui_Box_V_Sample_Interpolation_Labels, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_V_Sample_Interpolation_Labels);
        // Sample Interpolation Label, X
        Gui_Sample_Interpolation_X_Label = gtk_label_new(NULL);
        gtk_widget_set_size_request(Gui_Sample_Interpolation_X_Label, cell_r2_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Sample_Interpolation_X_Label), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Sample_Interpolation_X_Label), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Interpolation_Labels), (GtkWidget*)Gui_Sample_Interpolation_X_Label, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Sample_Interpolation_X_Label);
        gtk_label_set_text((GtkLabel*)Gui_Sample_Interpolation_X_Label, g_strdup_printf(_("Sample Interpolation X" "%s"), "") );
        // Sample Interpolation Label, Y
        Gui_Sample_Interpolation_Y_Label = gtk_label_new(NULL);
        gtk_widget_set_size_request(Gui_Sample_Interpolation_Y_Label, cell_r2_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Sample_Interpolation_Y_Label), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Sample_Interpolation_Y_Label), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Interpolation_Labels), (GtkWidget*)Gui_Sample_Interpolation_Y_Label, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Sample_Interpolation_Y_Label);
        gtk_label_set_text((GtkLabel*)Gui_Sample_Interpolation_Y_Label, g_strdup_printf(_("Sample Interpolation Y" "%s"), "") );

        // Row 3, Column 1 Box
        Gui_Box_V_Sample_Interpolation_SpinButtons = gtk_box_new(GTK_ORIENTATION_VERTICAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_V_Sample_Interpolation_SpinButtons), box_padding);
        gtk_widget_set_size_request(Gui_Box_V_Sample_Interpolation_SpinButtons, cell_r2_width, row_2_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_V_Sample_Interpolation_SpinButtons), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_V_Sample_Interpolation_SpinButtons), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_3), (GtkWidget*)Gui_Box_V_Sample_Interpolation_SpinButtons, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_V_Sample_Interpolation_SpinButtons);
        // Spin Button that determines the sample interpolation, X
        Gui_Sample_Interpolation_X_SpinButton = gimp_spin_button_new_with_range((gdouble) 0.0, (gdouble) 5.0000, (gdouble) 0.10);
        gtk_widget_set_size_request(Gui_Sample_Interpolation_X_SpinButton, cell_r2_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Sample_Interpolation_X_SpinButton), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Sample_Interpolation_X_SpinButton), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Interpolation_SpinButtons), (GtkWidget*)Gui_Sample_Interpolation_X_SpinButton, FALSE, FALSE, 0);
        gtk_spin_button_set_digits((GtkSpinButton*)Gui_Sample_Interpolation_X_SpinButton, (guint) 2);
        gtk_widget_show(Gui_Sample_Interpolation_X_SpinButton);
        gtk_spin_button_set_value((GtkSpinButton*)Gui_Sample_Interpolation_X_SpinButton, (gdouble)sample_interpolation_x);
        // Spin Button that determines the sample interpolation, Y
        Gui_Sample_Interpolation_Y_SpinButton = gimp_spin_button_new_with_range((gdouble) 0.0, (gdouble) 5.0000, (gdouble) 0.10);
        gtk_widget_set_size_request(Gui_Sample_Interpolation_Y_SpinButton, cell_r2_width, cell_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Sample_Interpolation_Y_SpinButton), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Sample_Interpolation_Y_SpinButton), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_V_Sample_Interpolation_SpinButtons), (GtkWidget*)Gui_Sample_Interpolation_Y_SpinButton, FALSE, FALSE, 0);
        gtk_spin_button_set_digits((GtkSpinButton*)Gui_Sample_Interpolation_Y_SpinButton, (guint) 2);
        gtk_widget_show(Gui_Sample_Interpolation_Y_SpinButton);
        gtk_spin_button_set_value((GtkSpinButton*)Gui_Sample_Interpolation_Y_SpinButton, (gdouble)sample_interpolation_y);
        // Chain Button that determines when the sample interpolation has it's aspect ratio locked
        Gui_Sample_Interpolation_ChainButton = gimp_chain_button_new(GIMP_CHAIN_RIGHT);
        gtk_widget_set_size_request(Gui_Sample_Interpolation_ChainButton, chain_button_width, cell_height * (gint)2);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Sample_Interpolation_ChainButton), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Sample_Interpolation_ChainButton), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_3), (GtkWidget*)Gui_Sample_Interpolation_ChainButton, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Sample_Interpolation_ChainButton);

        // Row 4 Box
        Gui_Box_H_Row_4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_4), box_padding);
        gtk_widget_set_size_request(Gui_Box_H_Row_4, master_width, row_3_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_H_Row_4), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_H_Row_4), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_4, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_H_Row_4);
        // Seamless Looping Border Button
        Gui_Button_Seamless_X = gtk_button_new_with_label("Regular Horizontal Borders");
        gtk_widget_set_size_request(Gui_Button_Seamless_X, cell_r3_width, row_3_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_Seamless_X), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_Seamless_X), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_4), (GtkWidget*)Gui_Button_Seamless_X, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_Seamless_X);
        // Seamless Looping Border Button
        Gui_Button_Seamless_Y = gtk_button_new_with_label("Regular Vertical Borders");
        gtk_widget_set_size_request(Gui_Button_Seamless_Y, cell_r3_width, row_3_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_Seamless_Y), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_Seamless_Y), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_4), (GtkWidget*)Gui_Button_Seamless_Y, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_Seamless_Y);

        // Row 5 Box
        Gui_Box_H_Row_5 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, (gint)0);
        gtk_container_set_border_width(GTK_CONTAINER (Gui_Box_H_Row_5), box_padding);
        gtk_widget_set_size_request(Gui_Box_H_Row_5, master_width, row_3_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Box_H_Row_5), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Box_H_Row_5), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_Master), (GtkWidget*)Gui_Box_H_Row_5, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Box_H_Row_5);
        // Plug-in Choice Button
        Gui_Button_Plugin_Run = gtk_button_new_with_label("Run");
        gtk_widget_set_size_request(Gui_Button_Plugin_Run, 150, row_3_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_Plugin_Run), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_Plugin_Run), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_5), (GtkWidget*)Gui_Button_Plugin_Run, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_Plugin_Run);
        // Plug-in Choice Button
        Gui_Button_Plugin_Cancel = gtk_button_new_with_label("Cancel / Quit");
        gtk_widget_set_size_request(Gui_Button_Plugin_Cancel, 150, row_3_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_Plugin_Cancel), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_Plugin_Cancel), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_5), (GtkWidget*)Gui_Button_Plugin_Cancel, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_Plugin_Cancel);
        // Plug-in Choice Button
        Gui_Button_Plugin_Reset = gtk_button_new_with_label("Full Reset");
        gtk_widget_set_size_request(Gui_Button_Plugin_Reset, 150, row_3_height);
        gtk_widget_set_halign(GTK_WIDGET(Gui_Button_Plugin_Reset), GTK_ALIGN_START);
        gtk_widget_set_valign(GTK_WIDGET(Gui_Button_Plugin_Reset), GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(Gui_Box_H_Row_5), (GtkWidget*)Gui_Button_Plugin_Reset, FALSE, FALSE, 0);
        gtk_widget_show(Gui_Button_Plugin_Reset);

        gtk_widget_show(dialog);

        SyncDataFromParameters();

        // ComboSizeWidget_Instance = this;
        self = this;

        button_pointer_Size_Reset = new int(1);
        button_pointer_m2 = new int(2);
        button_pointer_m5 = new int(3);
        button_pointer_m10 = new int(4);
        button_pointer_d2 = new int(5);
        button_pointer_d5 = new int(6);
        button_pointer_d10 = new int(7);
        button_pointer_flip_xy = new int(8);
        button_pointer_seamless_x = new int(17);
        button_pointer_seamless_y = new int(18);
        button_pointer_sample_grid_scale_Reset = new int(9);
        button_pointer_sample_grid_scale_125 = new int(10);
        button_pointer_sample_grid_scale_150 = new int(11);
        button_pointer_sample_grid_scale_200 = new int(12);
        button_pointer_sample_grid_scale_250 = new int(13);
        button_pointer_sample_grid_scale_325 = new int(14);
        button_pointer_sample_grid_scale_500 = new int(15);
        button_pointer_sample_grid_scale_1200 = new int(16);
        button_pointer_run_plugin = new int(21);
        button_pointer_cancel_plugin = new int(22);
        button_pointer_reset_plugin = new int(23);

        g_signal_connect(Gui_SpinButton_Size_X_Pixels, "value-changed", G_CALLBACK(value_changed_size_x), self);
        g_signal_connect(Gui_SpinButton_Size_Y_Pixels, "value-changed", G_CALLBACK(value_changed_size_y), self);
        g_signal_connect(Gui_SpinButton_Size_X_Scale, "value-changed", G_CALLBACK(value_changed_scale_x), self);
        g_signal_connect(Gui_SpinButton_Size_Y_Scale, "value-changed", G_CALLBACK(value_changed_scale_y), self);
        g_signal_connect(Gui_ChainButton, "toggled", G_CALLBACK(value_changed_chain_button), self);
        g_signal_connect(Gui_SpinButton_Sample_Grid_Scale_X, "value-changed", G_CALLBACK(value_changed_sample_grid_scale_x), self);
        g_signal_connect(Gui_SpinButton_Sample_Grid_Scale_Y, "value-changed", G_CALLBACK(value_changed_sample_grid_scale_y), self);
        g_signal_connect(Gui_ChainButton_Sample_Grid_Scale, "toggled", G_CALLBACK(value_changed_chain_button_sample_grid_scale), self);
        g_signal_connect(Gui_Sample_Interpolation_X_SpinButton, "value-changed", G_CALLBACK(value_changed_interpolation_x), self);
        g_signal_connect(Gui_Sample_Interpolation_Y_SpinButton, "value-changed", G_CALLBACK(value_changed_interpolation_y), self);
        g_signal_connect(Gui_Sample_Interpolation_ChainButton, "toggled", G_CALLBACK(value_changed_chain_button_sample_interpolation), self);

        g_object_set_data(G_OBJECT(Gui_Button_Size_Reset), "button-name", button_pointer_Size_Reset);
        g_object_set_data(G_OBJECT(Gui_Button_m2), "button-name", button_pointer_m2);
        g_object_set_data(G_OBJECT(Gui_Button_m5), "button-name", button_pointer_m5);
        g_object_set_data(G_OBJECT(Gui_Button_m10), "button-name", button_pointer_m10);
        g_object_set_data(G_OBJECT(Gui_Button_d2), "button-name", button_pointer_d2);
        g_object_set_data(G_OBJECT(Gui_Button_d5), "button-name", button_pointer_d5);
        g_object_set_data(G_OBJECT(Gui_Button_d10), "button-name", button_pointer_d10);
        g_object_set_data(G_OBJECT(Gui_Button_flip_xy), "button-name", button_pointer_flip_xy);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_Reset), "button-name", button_pointer_sample_grid_scale_Reset);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_125), "button-name", button_pointer_sample_grid_scale_125);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_150), "button-name", button_pointer_sample_grid_scale_150);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_200), "button-name", button_pointer_sample_grid_scale_200);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_250), "button-name", button_pointer_sample_grid_scale_250);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_325), "button-name", button_pointer_sample_grid_scale_325);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_500), "button-name", button_pointer_sample_grid_scale_500);
        g_object_set_data(G_OBJECT(Gui_Button_SampleGridScale_1200), "button-name", button_pointer_sample_grid_scale_1200);
        g_object_set_data(G_OBJECT(Gui_Button_Seamless_X), "button-name", button_pointer_seamless_x);
        g_object_set_data(G_OBJECT(Gui_Button_Seamless_Y), "button-name", button_pointer_seamless_y);
        g_object_set_data(G_OBJECT(Gui_Button_Plugin_Run), "button-name", button_pointer_run_plugin);
        g_object_set_data(G_OBJECT(Gui_Button_Plugin_Cancel), "button-name", button_pointer_cancel_plugin);
        g_object_set_data(G_OBJECT(Gui_Button_Plugin_Reset), "button-name", button_pointer_reset_plugin);

        g_signal_connect(Gui_Button_Size_Reset, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_m2, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_m5, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_m10, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_d2, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_d5, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_d10, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_flip_xy, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_Reset, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_125, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_150, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_200, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_250, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_325, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_500, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_SampleGridScale_1200, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Seamless_X, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Seamless_Y, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Plugin_Run, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Plugin_Cancel, "clicked", G_CALLBACK(value_changed_button), self);
        g_signal_connect(Gui_Button_Plugin_Reset, "clicked", G_CALLBACK(value_changed_button), self);
    }
    ComboSizeWidget::~ComboSizeWidget()
    {
        // ComboSizeWidget_Instance = NULL;
        delete button_pointer_Size_Reset;
        delete button_pointer_m2;
        delete button_pointer_m5;
        delete button_pointer_m10;
        delete button_pointer_d2;
        delete button_pointer_d5;
        delete button_pointer_d10;
        delete button_pointer_flip_xy;
        delete button_pointer_sample_grid_scale_Reset;
        delete button_pointer_sample_grid_scale_125;
        delete button_pointer_sample_grid_scale_150;
        delete button_pointer_sample_grid_scale_200;
        delete button_pointer_sample_grid_scale_250;
        delete button_pointer_sample_grid_scale_325;
        delete button_pointer_sample_grid_scale_500;
        delete button_pointer_sample_grid_scale_1200;
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

        size_x = (gint) max((gint)val, (gint)1);
        f128 scale_x_f = f128(size_x) / f128(original_x);
        scale_x = (gdouble) fmin(fmax((double)scale_x_f, 0.01), 99999.99);

        if (chain_button_on == TRUE)
        {
            f128 new_scale_val = f128(size_x) / locked_ratio_xy;
            size_y = (gint) max((gint)new_scale_val, (gint)1);

            f128 scl_y = f128(size_y) / f128(original_y);
            scale_y = (gdouble)scl_y;
        } else
        {
            locked_ratio_xy = f128(size_x) / f128(size_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetSizeY(gint val)
    {
        if (ignore_auto_changes == true) return;

        size_y = (gint) max((gint)val, (gint)1);
        f128 scale_y_f = f128(size_y) / f128(original_y);
        scale_y = (gdouble) fmin(fmax((double)scale_y_f, 0.01), 99999.99);

        if (chain_button_on == TRUE)
        {
            f128 new_scale_val = f128(size_y) * locked_ratio_xy;
            size_x = (gint) max((gint)new_scale_val, (gint)1);

            f128 scl_x = f128(size_x) / f128(original_x);
            scale_x = (gdouble)scl_x;
        } else
        {
            locked_ratio_xy = f128(size_x) / f128(size_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetScaleX(gdouble val)
    {
        if (ignore_auto_changes == true) return;

        scale_x = (gdouble) fmin(fmax((double)val * 0.01, 0.0001), 99999.99);
        f128 size_x_f = f128(original_x) * f128(scale_x);
        size_x = (gint) max((gint)size_x_f, (gint)1);

        if (chain_button_on == TRUE)
        {
            f128 new_scale_val = f128(size_x) / locked_ratio_xy;
            size_y = (gint) max((gint)new_scale_val, (gint)1);

            f128 scl_y = f128(size_y) / f128(original_y);
            scale_y = (gdouble)scl_y;
        } else
        {
            locked_ratio_xy = f128(size_x) / f128(size_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetScaleY(gdouble val)
    {
        if (ignore_auto_changes == true) return;

        scale_y = (gdouble) fmin(fmax((double)val * 0.01, 0.0001), 99999.99);
        f128 size_y_f = f128(original_y) * f128(scale_y);
        size_y = (gint) max((gint)size_y_f, (gint)1);

        if (chain_button_on == TRUE)
        {
            f128 new_scale_val = f128(size_y) * locked_ratio_xy;
            size_x = (gint) max((gint)new_scale_val, (gint)1);

            f128 scl_x = f128(size_x) / f128(original_x);
            scale_x = (gdouble)scl_x;
        } else
        {
            locked_ratio_xy = f128(size_x) / f128(size_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetScale_SampleGridScaleX(gdouble val)
    {
        if (ignore_auto_changes == true) return;

        sample_grid_scale_x = gdouble (min(max(val, 0.01), 5000.0) * 0.01);

        if (chain_button_sample_grid_scale_on == TRUE)
        {
            f128 sample_grid_scale_y_f = f128(sample_grid_scale_x / (f128)sample_grid_scale_locked_ratio_xy);
            sample_grid_scale_y = (gdouble)sample_grid_scale_y_f;
        } else
        {
            sample_grid_scale_locked_ratio_xy = f128(sample_grid_scale_x) / f128(sample_grid_scale_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetScale_SampleGridScaleY(gdouble val)
    {
        if (ignore_auto_changes == true) return;

        sample_grid_scale_y = gdouble (min(max(val, 0.01), 5000.0) * 0.01);

        if (chain_button_sample_grid_scale_on == TRUE)
        {
            f128 sample_grid_scale_x_f = f128(sample_grid_scale_y * (f128)sample_grid_scale_locked_ratio_xy);
            sample_grid_scale_x = (gdouble)sample_grid_scale_x_f;
        } else
        {
            sample_grid_scale_locked_ratio_xy = f128(sample_grid_scale_x) / f128(sample_grid_scale_y);
        }

        if (chain_button_sample_grid_scale_on == TRUE)
        {
        } else
        {
            sample_grid_scale_locked_ratio_xy = f128(sample_grid_scale_x) / f128(sample_grid_scale_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetSampleInterpolationX(gdouble val)
    {
        if (ignore_auto_changes == true) return;

        sample_interpolation_x = gdouble (min(max(val, 0.0), 50.0));

        if (chain_button_sample_interpolation_on == TRUE)
        {
            f128 sample_interpolation_y_f = f128(sample_interpolation_x / (f128)sample_interpolation_locked_ratio_xy);
            sample_interpolation_y = (gdouble)sample_interpolation_y_f;
        } else
        {
            sample_interpolation_locked_ratio_xy = fmaxq(f128(sample_interpolation_x), 0.00001_q) / fmaxq(f128(sample_interpolation_y), 0.00001_q);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetSampleInterpolationY(gdouble val)
    {
        if (ignore_auto_changes == true) return;

        sample_interpolation_y = gdouble (min(max(val, 0.0), 50.0));

        if (chain_button_sample_interpolation_on == TRUE)
        {
            f128 sample_interpolation_x_f = f128(sample_interpolation_y * (f128)sample_interpolation_locked_ratio_xy);
            sample_interpolation_x = (gdouble)sample_interpolation_x_f;
        } else
        {
            sample_interpolation_locked_ratio_xy = fmaxq(f128(sample_interpolation_x), 0.00001_q) / fmaxq(f128(sample_interpolation_y), 0.00001_q);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetChainButton(gboolean val)
    {
        if (ignore_auto_changes == true) return;

        if ((chain_button_on == TRUE) && (val == FALSE))
            locked_ratio_xy = f128(size_x) / f128(size_y);

        chain_button_on = (gboolean) val;

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetChainButton_SampleGridScale(gboolean val)
    {
        if (ignore_auto_changes == true) return;

        if ((chain_button_sample_grid_scale_on == TRUE) && (val == FALSE))
            sample_grid_scale_locked_ratio_xy = f128(sample_grid_scale_x) / f128(sample_grid_scale_y);

        chain_button_sample_grid_scale_on = (gboolean) val;

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetChainButton_SampleInterpolation(gboolean val)
    {
        if (ignore_auto_changes == true) return;

        if ((chain_button_sample_interpolation_on == TRUE) && (val == FALSE))
            sample_interpolation_locked_ratio_xy = fmaxq(f128(sample_interpolation_x), 0.00001_q) / fmaxq(f128(sample_interpolation_y), 0.00001_q);

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
            locked_ratio_xy = f128(size_x) / f128(size_y);
        }

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }
    void ComboSizeWidget::SetOriginalSize(gint val_x, gint val_y)
    {
        original_x = (gint) max((gint)val_x, (gint)1);
        original_y = (gint) max((gint)val_y, (gint)1);

        original_ratio_xy = f128(original_x) / f128(original_y);
        locked_ratio_xy = original_ratio_xy;
        size_x = original_x;
        size_y = original_y;
        scale_x = (gdouble) 1.0;
        scale_y = (gdouble) 1.0;

        if (Gui_Text_Size_Original_X)
            gtk_label_set_text((GtkLabel*)Gui_Text_Size_Original_X, g_strdup_printf(_("Width: %i" "%s"), original_x, "") );
        if (Gui_Text_Size_Original_Y)
            gtk_label_set_text((GtkLabel*)Gui_Text_Size_Original_Y, g_strdup_printf(_("Height: %i" "%s"), original_y, "") );

        ignore_auto_changes = true;
        UpdateGUI();
        ignore_auto_changes = false;
    }

    void ComboSizeWidget::UpdateGUI()
    {
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

        if (Gui_Sample_Interpolation_X_SpinButton)
            gtk_spin_button_set_value((GtkSpinButton*)Gui_Sample_Interpolation_X_SpinButton, (gdouble)sample_interpolation_x);
        if (Gui_Sample_Interpolation_Y_SpinButton)
            gtk_spin_button_set_value((GtkSpinButton*)Gui_Sample_Interpolation_Y_SpinButton, (gdouble)sample_interpolation_y);
        if (Gui_Sample_Interpolation_ChainButton)
            gimp_chain_button_set_active((GimpChainButton*)Gui_Sample_Interpolation_ChainButton, chain_button_sample_interpolation_on);

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

        SyncDataToParameters();

        if (Log && Params)
        {
            Params->CalcAll();
            Log->LogGui(false, g_strdup_printf( _("%s%s"), Params->info_string.c_str(), "-\n-\n-\n-\n-\n-\n-" ));
        }
    }
    void ComboSizeWidget::SyncDataToParameters()
    {
        if (Params)
        {
            Params->input_size_x = (int)original_x;
            Params->input_size_y = (int)original_y;
            Params->output_size_x = (int)size_x;
            Params->output_size_y = (int)size_y;
            Params->seamless_x = (seamless_x == TRUE) ? true : false;
            Params->seamless_y = (seamless_y == TRUE) ? true : false;
            Params->sample_grid_width_percent = (f64)sample_grid_scale_x * 100.0;
            Params->sample_grid_height_percent = (f64)sample_grid_scale_y * 100.0;
            Params->sample_interpolation_x = (f128)sample_interpolation_x;
            Params->sample_interpolation_y = (f128)sample_interpolation_y;
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

            original_ratio_xy = f128(original_x) / f128(original_y);
            locked_ratio_xy = f128(size_x) / f128(size_y);
            sample_grid_scale_locked_ratio_xy = f128(sample_grid_scale_x) / f128(sample_grid_scale_y);
            sample_interpolation_locked_ratio_xy = fmaxq(f128(sample_interpolation_x), 0.00001_q) / fmaxq(f128(sample_interpolation_y), 0.00001_q);

            seamless_x = (Params->seamless_x == true) ? TRUE : FALSE;
            seamless_y = (Params->seamless_y == true) ? TRUE : FALSE;

            if (Gui_Text_Size_Original_X)
                gtk_label_set_text((GtkLabel*)Gui_Text_Size_Original_X, g_strdup_printf(_("Width: %i" "%s"), original_x, "\n") );
            if (Gui_Text_Size_Original_Y)
                gtk_label_set_text((GtkLabel*)Gui_Text_Size_Original_Y, g_strdup_printf(_("Height: %i" "%s"), original_y, "\n") );

            UpdateGUI();

            ignore_auto_changes = false;
        }
    }
    void ComboSizeWidget::SetButtonClicked(int* button_name)
    {
        if (button_name == button_pointer_Size_Reset)
        {
            original_ratio_xy = f128(original_x) / f128(original_y);
            locked_ratio_xy = original_ratio_xy;
            size_x = original_x;
            size_y = original_y;
            scale_x = (gdouble) 1.0;
            scale_y = (gdouble) 1.0;
            sample_grid_scale_x = (gdouble) 1.0;
            sample_grid_scale_y = (gdouble) 1.0;
            sample_interpolation_x = (gdouble) 1.0;
            sample_interpolation_y = (gdouble) 1.0;

            sample_grid_scale_locked_ratio_xy = f128(sample_grid_scale_x) / f128(sample_grid_scale_y);
            sample_interpolation_locked_ratio_xy = fmaxq(f128(sample_interpolation_x), 0.00001_q) / fmaxq(f128(sample_interpolation_y), 0.00001_q);

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_m2)
        {
            size_x *= 2;
            size_y *= 2;
            scale_x *= 2.0;
            scale_y *= 2.0;

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_m5)
        {
            size_x *= 5;
            size_y *= 5;
            scale_x *= 5.0;
            scale_y *= 5.0;

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_m10)
        {
            size_x *= 10;
            size_y *= 10;
            scale_x *= 10.0;
            scale_y *= 10.0;

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_d2)
        {
            size_x /= 2;
            size_y /= 2;
            scale_x /= 2.0;
            scale_y /= 2.0;

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_d5)
        {
            size_x /= 5;
            size_y /= 5;
            scale_x /= 5.0;
            scale_y /= 5.0;

            ignore_auto_changes = true;
            UpdateGUI();
            ignore_auto_changes = false;
        }
        else if (button_name == button_pointer_d10)
        {
            size_x /= 10;
            size_y /= 10;
            scale_x /= 10.0;
            scale_y /= 10.0;

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
            f128 scale_x_f = f128(size_x) / f128(original_x);
            f128 scale_y_f = f128(size_y) / f128(original_y);
            scale_x = (gdouble) scale_x_f;
            scale_y = (gdouble) scale_y_f;
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
            seamless_x = FALSE;
            seamless_y = FALSE;

            SetButtonClicked(button_pointer_Size_Reset);
            SetButtonClicked(button_pointer_sample_grid_scale_Reset);
        }
    }
    int ComboSizeWidget::GetChoicesDoneResult()
    {
        return choices_done_result;
    }

}; // END namespace TritonhawkPlus
