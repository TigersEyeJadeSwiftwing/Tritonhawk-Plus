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

static inline __attribute__((always_inline, hot))
s8 clamp(s8 v, s8 lo, s8 hi)
{
    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

static inline __attribute__((always_inline, hot))
s16 clamp(s16 v, s16 lo, s16 hi)
{
    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

static inline __attribute__((always_inline, hot))
s32 clamp(s32 v, s32 lo, s32 hi)
{
    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

static inline __attribute__((always_inline, hot))
s64 clamp(s64 v, s64 lo, s64 hi)
{
    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

static inline __attribute__((always_inline, hot))
u8 clamp(u8 v, u8 lo, u8 hi)
{
    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

static inline __attribute__((always_inline, hot))
u16 clamp(u16 v, u16 lo, u16 hi)
{
    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

static inline __attribute__((always_inline, hot))
u32 clamp(u32 v, u32 lo, u32 hi)
{
    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}

static inline __attribute__((always_inline, hot))
u64 clamp(u64 v, u64 lo, u64 hi)
{
    return (v >= lo) ? ((v <= hi) ? v : hi) : lo;
}
