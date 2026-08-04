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

#ifndef THP_PLUG_IN_ENCHANT
#define THP_PLUG_IN_ENCHANT

static GimpValueArray*  thp_image_magic_enchant_run(
                            GimpProcedure* procedure,
                            GimpRunMode run_mode,
                            GimpImage* image,
                            GimpDrawable** drawables,
                            GimpProcedureConfig* config,
                            gpointer run_data
                        )
{
    Params = new ThpParams();

    Params->process_name = (string)THP_PLUG_IN_PROCESS_Image_Magic_Enchant;

    Params->hardware_max_threads = omp_get_max_threads();
    Params->preferences_max_threads = gimp_get_num_processors();
    Params->number_threads = std::min(Params->hardware_max_threads, Params->preferences_max_threads);
    omp_set_num_threads( (int)Params->number_threads );

    Params->procedure = procedure;
    Params->run_mode_interactive = (run_mode == GIMP_RUN_INTERACTIVE) ? true : false;
    Params->image = image;
    Params->drawables = drawables;
    Params->config = config;

    Params->images_to_process = IMAGES_TO_PROCESS__CURRENT;
    Params->layers_to_process = LAYERS_TO_PROCESS__ALL_LAYERS;
    Params->run_mode = RUN_MODE_RESIZE__ALL_LAYERS_SAME_RATIO_V2;
    Params->multiple_image_layers = true;
    Params->sample_grid_is_for_all_layers = true;

    bool result = Thp_Operation__Resize();

    if (result == true)
    {
        return gimp_procedure_new_return_values(procedure, GIMP_PDB_SUCCESS, NULL);
    }
    else
    {
        GError *error = NULL;
        g_set_error(
            &error,
            GIMP_PLUG_IN_ERROR,
            0,
            _("\n----- Error with Procedure '%s':" "\n" "%s"),
            Params->process_name.c_str(),
            Params->operation_result_string.c_str()
        );

        clean(Log);
        clean(Params);

        return gimp_procedure_new_return_values(procedure, GIMP_PDB_CALLING_ERROR, error);
    }
};

#endif // THP_PLUG_IN_ENCHANT
