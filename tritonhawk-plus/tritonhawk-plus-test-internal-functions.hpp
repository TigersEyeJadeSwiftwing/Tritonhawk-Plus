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

#ifndef THP_PLUG_IN_TEST_INTERNAL_FUNCTIONS
#define THP_PLUG_IN_TEST_INTERNAL_FUNCTIONS

#include "components/thp_types.hpp"

#include <string>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;
using namespace TritonhawkPlus;
using namespace quadmath;

static GimpValueArray*  thp_test_internal_functions(
                            GimpProcedure* procedure,
                            GimpRunMode run_mode,
                            GimpImage* image,
                            GimpDrawable** drawables,
                            GimpProcedureConfig* config,
                            gpointer run_data
                        )
{
    string test_floorq = g_strdup_printf
    (
        _(
            "\n"
            "floor(0.9): %lf    floorq(0.9): %lf" "\n"
            "floor(1.9): %lf    floorq(1.9): %lf" "\n"
            "floor(2.9): %lf    floorq(2.9): %lf" "\n"
            "floor(0.2): %lf    floorq(0.2): %lf" "\n"
            "floor(1.2): %lf    floorq(1.2): %lf" "\n"
            "floor(2.2): %lf    floorq(2.2): %lf" "\n"
            "floor(-0.9): %lf    floorq(-0.9): %lf" "\n"
            "floor(-1.9): %lf    floorq(-1.9): %lf" "\n"
            "floor(-2.9): %lf    floorq(-2.9): %lf" "\n"
            "floor(-0.2): %lf    floorq(-0.2): %lf" "\n"
            "floor(-1.2): %lf    floorq(-1.2): %lf" "\n"
            "floor(-2.2): %lf    floorq(-2.2): %lf" "\n"
            "\n"
        ),
        floor(0.9), (double)floorq(0.9),
        floor(1.9), (double)floorq(1.9),
        floor(2.9), (double)floorq(2.9),
        floor(0.2), (double)floorq(0.2),
        floor(1.2), (double)floorq(1.2),
        floor(2.2), (double)floorq(2.2),
        floor(-0.9), (double)floorq(-0.9),
        floor(-1.9), (double)floorq(-1.9),
        floor(-2.9), (double)floorq(-2.9),
        floor(-0.2), (double)floorq(-0.2),
        floor(-1.2), (double)floorq(-1.2),
        floor(-2.2), (double)floorq(-2.2)
    );

    string test_ceilq = g_strdup_printf
    (
        _(
            "%s"
            "ceil(0.9): %lf    ceilq(0.9): %lf" "\n"
            "ceil(1.9): %lf    ceilq(1.9): %lf" "\n"
            "ceil(2.9): %lf    ceilq(2.9): %lf" "\n"
            "ceil(0.2): %lf    ceilq(0.2): %lf" "\n"
            "ceil(1.2): %lf    ceilq(1.2): %lf" "\n"
            "ceil(2.2): %lf    ceilq(2.2): %lf" "\n"
            "ceil(-0.9): %lf    ceilq(-0.9): %lf" "\n"
            "ceil(-1.9): %lf    ceilq(-1.9): %lf" "\n"
            "ceil(-2.9): %lf    ceilq(-2.9): %lf" "\n"
            "ceil(-0.2): %lf    ceilq(-0.2): %lf" "\n"
            "ceil(-1.2): %lf    ceilq(-1.2): %lf" "\n"
            "ceil(-2.2): %lf    ceilq(-2.2): %lf" "\n"
            "\n"
        ),
        test_floorq.c_str(),
        ceil(0.9), (double)ceilq(0.9),
        ceil(1.9), (double)ceilq(1.9),
        ceil(2.9), (double)ceilq(2.9),
        ceil(0.2), (double)ceilq(0.2),
        ceil(1.2), (double)ceilq(1.2),
        ceil(2.2), (double)ceilq(2.2),
        ceil(-0.9), (double)ceilq(-0.9),
        ceil(-1.9), (double)ceilq(-1.9),
        ceil(-2.9), (double)ceilq(-2.9),
        ceil(-0.2), (double)ceilq(-0.2),
        ceil(-1.2), (double)ceilq(-1.2),
        ceil(-2.2), (double)ceilq(-2.2)
    );

    string test_clampq = g_strdup_printf
    (
        _(
            "%s"
            "clampq(5.25, 1.0, 10.0): %lf" "\n"
            "clampq(5.25, 9.0, 10.0): %lf" "\n"
            "clampq(5.25, 1.0, 2.0): %lf" "\n"
            "clampq(16.8, 1.0, 10.0): %lf" "\n"
            "\n"
        ),
        test_ceilq.c_str(),
        (double)clampq(5.25, 1.0, 10.0),
        (double)clampq(5.25, 9.0, 10.0),
        (double)clampq(5.25, 1.0, 2.0),
        (double)clampq(16.8, 1.0, 10.0)
    );

    string test_copysignq = g_strdup_printf
    (
        _(
            "%s"
            "copysignq(1.25, 1.25): %lf" "\n"
            "copysignq(1.25, -1.25): %lf" "\n"
            "copysignq(-1.25, 1.25): %lf" "\n"
            "copysignq(-1.25, -1.25): %lf" "\n"
            "\n"
        ),
        test_clampq.c_str(),
        (double)copysignq(1.25, 1.25),
        (double)copysignq(1.25, -1.25),
        (double)copysignq(-1.25, 1.25),
        (double)copysignq(-1.25, -1.25)
    );

    string test_nearbyintq = g_strdup_printf
    (
        _(
            "%s"
            "nearbyint(0.23): %lf    nearbyintq(0.23): %lf" "\n"
            "nearbyint(0.94): %lf    nearbyintq(0.94): %lf" "\n"
            "nearbyint(1.23): %lf    nearbyintq(1.23): %lf" "\n"
            "nearbyint(1.94): %lf    nearbyintq(1.94): %lf" "\n"
            "nearbyint(2.23): %lf    nearbyintq(2.23): %lf" "\n"
            "nearbyint(2.94): %lf    nearbyintq(2.94): %lf" "\n"
            "nearbyint(3.12): %lf    nearbyintq(3.12): %lf" "\n"
            "nearbyint(-0.23): %lf    nearbyintq(-0.23): %lf" "\n"
            "nearbyint(-0.94): %lf    nearbyintq(-0.94): %lf" "\n"
            "nearbyint(-1.23): %lf    nearbyintq(-1.23): %lf" "\n"
            "nearbyint(-1.94): %lf    nearbyintq(-1.94): %lf" "\n"
            "nearbyint(-2.23): %lf    nearbyintq(-2.23): %lf" "\n"
            "nearbyint(-2.94): %lf    nearbyintq(-2.94): %lf" "\n"
            "nearbyint(-3.12): %lf    nearbyintq(-3.12): %lf" "\n"
            "\n"
        ),
        test_copysignq.c_str(),
        nearbyint(0.23), (double)nearbyintq(0.23),
        nearbyint(0.94), (double)nearbyintq(0.94),
        nearbyint(1.23), (double)nearbyintq(1.23),
        nearbyint(1.94), (double)nearbyintq(1.94),
        nearbyint(2.23), (double)nearbyintq(2.23),
        nearbyint(2.94), (double)nearbyintq(2.94),
        nearbyint(3.12), (double)nearbyintq(3.12),
        nearbyint(-0.23), (double)nearbyintq(-0.23),
        nearbyint(-0.94), (double)nearbyintq(-0.94),
        nearbyint(-1.23), (double)nearbyintq(-1.23),
        nearbyint(-1.94), (double)nearbyintq(-1.94),
        nearbyint(-2.23), (double)nearbyintq(-2.23),
        nearbyint(-2.94), (double)nearbyintq(-2.94),
        nearbyint(-3.12), (double)nearbyintq(-3.12)
    );

    string test_fmaq = g_strdup_printf
    (
        _(
            "%s"
            "fma(0.0, 0.0, 0.0): %lf    fmaq(0.0, 0.0, 0.0): %lf" "\n"
            "fma(1.0, 1.0, 1.0): %lf    fmaq(1.0, 1.0, 1.0): %lf" "\n"
            "fma(2.0, 2.0, 2.0): %lf    fmaq(2.0, 2.0, 2.0): %lf" "\n"
            "fma(3.0, 3.0, 3.0): %lf    fmaq(3.0, 3.0, 3.0): %lf" "\n"
            "fma(4.0, 4.0, 4.0): %lf    fmaq(4.0, 4.0, 4.0): %lf" "\n"
            "fma(-1.0, -1.0, -1.0): %lf    fmaq(-1.0, -1.0, -1.0): %lf" "\n"
            "fma(-2.0, -2.0, -2.0): %lf    fmaq(-2.0, -2.0, -2.0): %lf" "\n"
            "fma(-3.0, -3.0, -3.0): %lf    fmaq(-3.0, -3.0, -3.0): %lf" "\n"
            "fma(-4.0, -4.0, -4.0): %lf    fmaq(-4.0, -4.0, -4.0): %lf" "\n"
            "fma(1.23, 1.23, 1.23): %lf    fmaq(1.23, 1.23, 1.23): %lf" "\n"
            "fma(2.34, 2.34, 2.34): %lf    fmaq(2.34, 2.34, 2.34): %lf" "\n"
            "fma(-1.23, -1.23, -1.23): %lf    fmaq(-1.23, -1.23, -1.23): %lf" "\n"
            "fma(-2.34, -2.34, -2.34): %lf    fmaq(-2.34, -2.34, -2.34): %lf" "\n"
            "\n"
        ),
        test_nearbyintq.c_str(),
        fma(0.0, 0.0, 0.0), (double)fmaq(0.0, 0.0, 0.0),
        fma(1.0, 1.0, 1.0), (double)fmaq(1.0, 1.0, 1.0),
        fma(2.0, 2.0, 2.0), (double)fmaq(2.0, 2.0, 2.0),
        fma(3.0, 3.0, 3.0), (double)fmaq(3.0, 3.0, 3.0),
        fma(4.0, 4.0, 4.0), (double)fmaq(4.0, 4.0, 4.0),
        fma(-1.0, -1.0, -1.0), (double)fmaq(-1.0, -1.0, -1.0),
        fma(-2.0, -2.0, -2.0), (double)fmaq(-2.0, -2.0, -2.0),
        fma(-3.0, -3.0, -3.0), (double)fmaq(-3.0, -3.0, -3.0),
        fma(-4.0, -4.0, -4.0), (double)fmaq(-4.0, -4.0, -4.0),
        fma(1.23, 1.23, 1.23), (double)fmaq(1.23, 1.23, 1.23),
        fma(2.34, 2.34, 2.34), (double)fmaq(2.34, 2.34, 2.34),
        fma(-1.23, -1.23, -1.23), (double)fmaq(-1.23, -1.23, -1.23),
        fma(-2.34, -2.34, -2.34), (double)fmaq(-2.34, -2.34, -2.34)
    );

    string test_sinq = g_strdup_printf
    (
        _(
            "%s"
            "sin(0.0): %lf    sinq(0.0): %lf" "\n"
            "sin(3.14): %lf    sinq(3.14): %lf" "\n"
            "sin(1.57): %lf    sinq(1.57): %lf" "\n"
            "sin(-1.57): %lf    sinq(-1.57): %lf" "\n"
            "sin(-3.14): %lf    sinq(-3.14): %lf" "\n"
            "sin(1.0): %lf    sinq(1.0): %lf" "\n"
            "sin(2.0): %lf    sinq(2.0): %lf" "\n"
            "sin(3.0): %lf    sinq(3.0): %lf" "\n"
            "sin(4.0): %lf    sinq(4.0): %lf" "\n"
            "sin(5.0): %lf    sinq(5.0): %lf" "\n"
            "sin(6.0): %lf    sinq(6.0): %lf" "\n"
            "sin(-1.0): %lf    sinq(-1.0): %lf" "\n"
            "sin(-2.0): %lf    sinq(-2.0): %lf" "\n"
            "sin(-3.0): %lf    sinq(-3.0): %lf" "\n"
            "sin(-4.0): %lf    sinq(-4.0): %lf" "\n"
            "sin(-5.0): %lf    sinq(-5.0): %lf" "\n"
            "sin(-6.0): %lf    sinq(-6.0): %lf" "\n"
            "\n"
        ),
        test_fmaq.c_str(),
        sin(0.0), (double)sinq(0.0),
        sin(3.14), (double)sinq(3.14),
        sin(1.57), (double)sinq(1.57),
        sin(-1.57), (double)sinq(-1.57),
        sin(-3.14), (double)sinq(-3.14),
        sin(1.0), (double)sinq(1.0),
        sin(2.0), (double)sinq(2.0),
        sin(3.0), (double)sinq(3.0),
        sin(4.0), (double)sinq(4.0),
        sin(5.0), (double)sinq(5.0),
        sin(6.0), (double)sinq(6.0),
        sin(-1.0), (double)sinq(-1.0),
        sin(-2.0), (double)sinq(-2.0),
        sin(-3.0), (double)sinq(-3.0),
        sin(-4.0), (double)sinq(-4.0),
        sin(-5.0), (double)sinq(-5.0),
        sin(-6.0), (double)sinq(-6.0)
    );

    string test_cosq = g_strdup_printf
    (
        _(
            "%s"
            "cos(0.0): %lf    cosq(0.0): %lf" "\n"
            "cos(3.14): %lf    cosq(3.14): %lf" "\n"
            "cos(1.57): %lf    cosq(1.57): %lf" "\n"
            "cos(-1.57): %lf    cosq(-1.57): %lf" "\n"
            "cos(-3.14): %lf    cosq(-3.14): %lf" "\n"
            "cos(1.0): %lf    cosq(1.0): %lf" "\n"
            "cos(2.0): %lf    cosq(2.0): %lf" "\n"
            "cos(3.0): %lf    cosq(3.0): %lf" "\n"
            "cos(4.0): %lf    cosq(4.0): %lf" "\n"
            "cos(5.0): %lf    cosq(5.0): %lf" "\n"
            "cos(6.0): %lf    cosq(6.0): %lf" "\n"
            "cos(-1.0): %lf    cosq(-1.0): %lf" "\n"
            "cos(-2.0): %lf    cosq(-2.0): %lf" "\n"
            "cos(-3.0): %lf    cosq(-3.0): %lf" "\n"
            "cos(-4.0): %lf    cosq(-4.0): %lf" "\n"
            "cos(-5.0): %lf    cosq(-5.0): %lf" "\n"
            "cos(-6.0): %lf    cosq(-6.0): %lf" "\n"
            "\n"
        ),
        test_sinq.c_str(),
        cos(0.0), (double)cosq(0.0),
        cos(3.14), (double)cosq(3.14),
        cos(1.57), (double)cosq(1.57),
        cos(-1.57), (double)cosq(-1.57),
        cos(-3.14), (double)cosq(-3.14),
        cos(1.0), (double)cosq(1.0),
        cos(2.0), (double)cosq(2.0),
        cos(3.0), (double)cosq(3.0),
        cos(4.0), (double)cosq(4.0),
        cos(5.0), (double)cosq(5.0),
        cos(6.0), (double)cosq(6.0),
        cos(-1.0), (double)cosq(-1.0),
        cos(-2.0), (double)cosq(-2.0),
        cos(-3.0), (double)cosq(-3.0),
        cos(-4.0), (double)cosq(-4.0),
        cos(-5.0), (double)cosq(-5.0),
        cos(-6.0), (double)cosq(-6.0)
    );

    string test_tanq = g_strdup_printf
    (
        _(
            "%s"
            "tan(0.0): %lf    tanq(0.0): %lf" "\n"
            "tan(3.14): %lf    tanq(3.14): %lf" "\n"
            "tan(1.57): %lf    tanq(1.57): %lf" "\n"
            "tan(-1.57): %lf    tanq(-1.57): %lf" "\n"
            "tan(-3.14): %lf    tanq(-3.14): %lf" "\n"
            "tan(1.0): %lf    tanq(1.0): %lf" "\n"
            "tan(2.0): %lf    tanq(2.0): %lf" "\n"
            "tan(3.0): %lf    tanq(3.0): %lf" "\n"
            "tan(4.0): %lf    tanq(4.0): %lf" "\n"
            "tan(5.0): %lf    tanq(5.0): %lf" "\n"
            "tan(6.0): %lf    tanq(6.0): %lf" "\n"
            "tan(-1.0): %lf    tanq(-1.0): %lf" "\n"
            "tan(-2.0): %lf    tanq(-2.0): %lf" "\n"
            "tan(-3.0): %lf    tanq(-3.0): %lf" "\n"
            "tan(-4.0): %lf    tanq(-4.0): %lf" "\n"
            "tan(-5.0): %lf    tanq(-5.0): %lf" "\n"
            "tan(-6.0): %lf    tanq(-6.0): %lf" "\n"
            "\n"
        ),
        test_cosq.c_str(),
        tan(0.0), (double)tanq(0.0),
        tan(3.14), (double)tanq(3.14),
        tan(1.57), (double)tanq(1.57),
        tan(-1.57), (double)tanq(-1.57),
        tan(-3.14), (double)tanq(-3.14),
        tan(1.0), (double)tanq(1.0),
        tan(2.0), (double)tanq(2.0),
        tan(3.0), (double)tanq(3.0),
        tan(4.0), (double)tanq(4.0),
        tan(5.0), (double)tanq(5.0),
        tan(6.0), (double)tanq(6.0),
        tan(-1.0), (double)tanq(-1.0),
        tan(-2.0), (double)tanq(-2.0),
        tan(-3.0), (double)tanq(-3.0),
        tan(-4.0), (double)tanq(-4.0),
        tan(-5.0), (double)tanq(-5.0),
        tan(-6.0), (double)tanq(-6.0)
    );

    string test_expq = g_strdup_printf
    (
        _(
            "%s"
            "exp(0.0): %lf    expq(0.0): %lf" "\n"
            "exp(3.14): %lf    expq(3.14): %lf" "\n"
            "exp(1.57): %lf    expq(1.57): %lf" "\n"
            "exp(-1.57): %lf    expq(-1.57): %lf" "\n"
            "exp(-3.14): %lf    expq(-3.14): %lf" "\n"
            "exp(1.0): %lf    expq(1.0): %lf" "\n"
            "exp(2.0): %lf    expq(2.0): %lf" "\n"
            "exp(3.0): %lf    expq(3.0): %lf" "\n"
            "exp(4.0): %lf    expq(4.0): %lf" "\n"
            "exp(5.0): %lf    expq(5.0): %lf" "\n"
            "exp(6.0): %lf    expq(6.0): %lf" "\n"
            "exp(-1.0): %lf    expq(-1.0): %lf" "\n"
            "exp(-2.0): %lf    expq(-2.0): %lf" "\n"
            "exp(-3.0): %lf    expq(-3.0): %lf" "\n"
            "exp(-4.0): %lf    expq(-4.0): %lf" "\n"
            "exp(-5.0): %lf    expq(-5.0): %lf" "\n"
            "exp(-6.0): %lf    expq(-6.0): %lf" "\n"
            "\n"
        ),
        test_tanq.c_str(),
        exp(0.0), (double)expq(0.0),
        exp(3.14), (double)expq(3.14),
        exp(1.57), (double)expq(1.57),
        exp(-1.57), (double)expq(-1.57),
        exp(-3.14), (double)expq(-3.14),
        exp(1.0), (double)expq(1.0),
        exp(2.0), (double)expq(2.0),
        exp(3.0), (double)expq(3.0),
        exp(4.0), (double)expq(4.0),
        exp(5.0), (double)expq(5.0),
        exp(6.0), (double)expq(6.0),
        exp(-1.0), (double)expq(-1.0),
        exp(-2.0), (double)expq(-2.0),
        exp(-3.0), (double)expq(-3.0),
        exp(-4.0), (double)expq(-4.0),
        exp(-5.0), (double)expq(-5.0),
        exp(-6.0), (double)expq(-6.0)
    );

    string test_logq = g_strdup_printf
    (
        _(
            "%s"
            "log(0.0): %lf    logq(0.0): %lf" "\n"
            "log(3.14): %lf    logq(3.14): %lf" "\n"
            "log(1.57): %lf    logq(1.57): %lf" "\n"
            "log(-1.57): %lf    logq(-1.57): %lf" "\n"
            "log(-3.14): %lf    logq(-3.14): %lf" "\n"
            "log(1.0): %lf    logq(1.0): %lf" "\n"
            "log(2.0): %lf    logq(2.0): %lf" "\n"
            "log(3.0): %lf    logq(3.0): %lf" "\n"
            "log(4.0): %lf    logq(4.0): %lf" "\n"
            "log(5.0): %lf    logq(5.0): %lf" "\n"
            "log(6.0): %lf    logq(6.0): %lf" "\n"
            "log(-1.0): %lf    logq(-1.0): %lf" "\n"
            "log(-2.0): %lf    logq(-2.0): %lf" "\n"
            "log(-3.0): %lf    logq(-3.0): %lf" "\n"
            "log(-4.0): %lf    logq(-4.0): %lf" "\n"
            "log(-5.0): %lf    logq(-5.0): %lf" "\n"
            "log(-6.0): %lf    logq(-6.0): %lf" "\n"
            "\n"
        ),
        test_expq.c_str(),
        log(0.0), (double)logq(0.0),
        log(3.14), (double)logq(3.14),
        log(1.57), (double)logq(1.57),
        log(-1.57), (double)logq(-1.57),
        log(-3.14), (double)logq(-3.14),
        log(1.0), (double)logq(1.0),
        log(2.0), (double)logq(2.0),
        log(3.0), (double)logq(3.0),
        log(4.0), (double)logq(4.0),
        log(5.0), (double)logq(5.0),
        log(6.0), (double)logq(6.0),
        log(-1.0), (double)logq(-1.0),
        log(-2.0), (double)logq(-2.0),
        log(-3.0), (double)logq(-3.0),
        log(-4.0), (double)logq(-4.0),
        log(-5.0), (double)logq(-5.0),
        log(-6.0), (double)logq(-6.0)
    );

    string test_powq_a = g_strdup_printf
    (
        _(
            "%s"
            "pow(0.0, 0.0): %lf    powq(0.0, 0.0): %lf" "\n"
            "pow(0.0, 1.0): %lf    powq(0.0, 1.0): %lf" "\n"
            "pow(0.0, 2.0): %lf    powq(0.0, 2.0): %lf" "\n"
            "pow(0.0, 3.0): %lf    powq(0.0, 3.0): %lf" "\n"
            "pow(0.0, 1.524): %lf    powq(0.0, 1.524): %lf" "\n"
            "pow(0.0, 3.691): %lf    powq(0.0, 3.691): %lf" "\n"

            "pow(1.0, 0.0): %lf    powq(1.0, 0.0): %lf" "\n"
            "pow(1.0, 1.0): %lf    powq(1.0, 1.0): %lf" "\n"
            "pow(1.0, 2.0): %lf    powq(1.0, 2.0): %lf" "\n"
            "pow(1.0, 3.0): %lf    powq(1.0, 3.0): %lf" "\n"
            "pow(1.0, 1.524): %lf    powq(1.0, 1.524): %lf" "\n"
            "pow(1.0, 3.691): %lf    powq(1.0, 3.691): %lf" "\n"

            "pow(2.964, 0.0): %lf    powq(2.964, 0.0): %lf" "\n"
            "pow(2.964, 1.0): %lf    powq(2.964, 1.0): %lf" "\n"
            "pow(2.964, 2.0): %lf    powq(2.964, 2.0): %lf" "\n"
            "pow(2.964, 3.0): %lf    powq(2.964, 3.0): %lf" "\n"
            "pow(2.964, 1.524): %lf    powq(2.964, 1.524): %lf" "\n"
            "pow(2.964, 3.691): %lf    powq(2.964, 3.691): %lf" "\n"

            "pow(4.592, 0.0): %lf    powq(4.592, 0.0): %lf" "\n"
            "pow(4.592, 1.0): %lf    powq(4.592, 1.0): %lf" "\n"
            "pow(4.592, 2.0): %lf    powq(4.592, 2.0): %lf" "\n"
            "pow(4.592, 3.0): %lf    powq(4.592, 3.0): %lf" "\n"
            "pow(4.592, 1.524): %lf    powq(4.592, 1.524): %lf" "\n"
            "pow(4.592, 3.691): %lf    powq(4.592, 3.691): %lf" "\n"

            "\n"
        ),
        test_logq.c_str(),

        pow(0.0, 0.0), (double)powq(0.0, 0.0),
        pow(0.0, 1.0), (double)powq(0.0, 1.0),
        pow(0.0, 2.0), (double)powq(0.0, 2.0),
        pow(0.0, 3.0), (double)powq(0.0, 3.0),
        pow(0.0, 1.524), (double)powq(0.0, 1.524),
        pow(0.0, 3.691), (double)powq(0.0, 3.691),

        pow(1.0, 0.0), (double)powq(1.0, 0.0),
        pow(1.0, 1.0), (double)powq(1.0, 1.0),
        pow(1.0, 2.0), (double)powq(1.0, 2.0),
        pow(1.0, 3.0), (double)powq(1.0, 3.0),
        pow(1.0, 1.524), (double)powq(1.0, 1.524),
        pow(1.0, 3.691), (double)powq(1.0, 3.691),

        pow(2.964, 0.0), (double)powq(2.964, 0.0),
        pow(2.964, 1.0), (double)powq(2.964, 1.0),
        pow(2.964, 2.0), (double)powq(2.964, 2.0),
        pow(2.964, 3.0), (double)powq(2.964, 3.0),
        pow(2.964, 1.524), (double)powq(2.964, 1.524),
        pow(2.964, 3.691), (double)powq(2.964, 3.691),

        pow(4.592, 0.0), (double)powq(4.592, 0.0),
        pow(4.592, 1.0), (double)powq(4.592, 1.0),
        pow(4.592, 2.0), (double)powq(4.592, 2.0),
        pow(4.592, 3.0), (double)powq(4.592, 3.0),
        pow(4.592, 1.524), (double)powq(4.592, 1.524),
        pow(4.592, 3.691), (double)powq(4.592, 3.691)
    );

    string test_powq_b = g_strdup_printf
    (
        _(
            "%s"
            "pow(-0.0, 0.0): %lf    powq(-0.0, 0.0): %lf" "\n"
            "pow(-0.0, 1.0): %lf    powq(-0.0, 1.0): %lf" "\n"
            "pow(-0.0, 2.0): %lf    powq(-0.0, 2.0): %lf" "\n"
            "pow(-0.0, 3.0): %lf    powq(-0.0, 3.0): %lf" "\n"
            "pow(-0.0, 1.524): %lf    powq(-0.0, 1.524): %lf" "\n"
            "pow(-0.0, 3.691): %lf    powq(-0.0, 3.691): %lf" "\n"

            "pow(-1.0, 0.0): %lf    powq(-1.0, 0.0): %lf" "\n"
            "pow(-1.0, 1.0): %lf    powq(-1.0, 1.0): %lf" "\n"
            "pow(-1.0, 2.0): %lf    powq(-1.0, 2.0): %lf" "\n"
            "pow(-1.0, 3.0): %lf    powq(-1.0, 3.0): %lf" "\n"
            "pow(-1.0, 1.524): %lf    powq(-1.0, 1.524): %lf" "\n"
            "pow(-1.0, 3.691): %lf    powq(-1.0, 3.691): %lf" "\n"

            "pow(-2.964, 0.0): %lf    powq(-2.964, 0.0): %lf" "\n"
            "pow(-2.964, 1.0): %lf    powq(-2.964, 1.0): %lf" "\n"
            "pow(-2.964, 2.0): %lf    powq(-2.964, 2.0): %lf" "\n"
            "pow(-2.964, 3.0): %lf    powq(-2.964, 3.0): %lf" "\n"
            "pow(-2.964, 1.524): %lf    powq(-2.964, 1.524): %lf" "\n"
            "pow(-2.964, 3.691): %lf    powq(-2.964, 3.691): %lf" "\n"

            "pow(-4.592, 0.0): %lf    powq(-4.592, 0.0): %lf" "\n"
            "pow(-4.592, 1.0): %lf    powq(-4.592, 1.0): %lf" "\n"
            "pow(-4.592, 2.0): %lf    powq(-4.592, 2.0): %lf" "\n"
            "pow(-4.592, 3.0): %lf    powq(-4.592, 3.0): %lf" "\n"
            "pow(-4.592, 1.524): %lf    powq(-4.592, 1.524): %lf" "\n"
            "pow(-4.592, 3.691): %lf    powq(-4.592, 3.691): %lf" "\n"

            "\n"
        ),
        test_powq_a.c_str(),

        pow(-0.0, 0.0), (double)powq(-0.0, 0.0),
        pow(-0.0, 1.0), (double)powq(-0.0, 1.0),
        pow(-0.0, 2.0), (double)powq(-0.0, 2.0),
        pow(-0.0, 3.0), (double)powq(-0.0, 3.0),
        pow(-0.0, 1.524), (double)powq(-0.0, 1.524),
        pow(-0.0, 3.691), (double)powq(-0.0, 3.691),

        pow(-1.0, 0.0), (double)powq(-1.0, 0.0),
        pow(-1.0, 1.0), (double)powq(-1.0, 1.0),
        pow(-1.0, 2.0), (double)powq(-1.0, 2.0),
        pow(-1.0, 3.0), (double)powq(-1.0, 3.0),
        pow(-1.0, 1.524), (double)powq(-1.0, 1.524),
        pow(-1.0, 3.691), (double)powq(-1.0, 3.691),

        pow(-2.964, 0.0), (double)powq(-2.964, 0.0),
        pow(-2.964, 1.0), (double)powq(-2.964, 1.0),
        pow(-2.964, 2.0), (double)powq(-2.964, 2.0),
        pow(-2.964, 3.0), (double)powq(-2.964, 3.0),
        pow(-2.964, 1.524), (double)powq(-2.964, 1.524),
        pow(-2.964, 3.691), (double)powq(-2.964, 3.691),

        pow(-4.592, 0.0), (double)powq(-4.592, 0.0),
        pow(-4.592, 1.0), (double)powq(-4.592, 1.0),
        pow(-4.592, 2.0), (double)powq(-4.592, 2.0),
        pow(-4.592, 3.0), (double)powq(-4.592, 3.0),
        pow(-4.592, 1.524), (double)powq(-4.592, 1.524),
        pow(-4.592, 3.691), (double)powq(-4.592, 3.691)
    );

    string test_results = g_strdup_printf
    (
        _(
            "%s"
            "ALL TESTS COMPLETE." "\n"
            "\n"
        ),
        test_powq_b.c_str()
    );

    {
        GError *error = NULL;
        g_set_error(
            &error,
            GIMP_PLUG_IN_ERROR,
            0,
            _("\n----- Test of internal functions:" "\n" "%s"),
            test_results.c_str()
        );

        return gimp_procedure_new_return_values(procedure, GIMP_PDB_CALLING_ERROR, error);
    }
}

#endif // THP_PLUG_IN_TEST_INTERNAL_FUNCTIONS
