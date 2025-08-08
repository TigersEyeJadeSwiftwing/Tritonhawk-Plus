#pragma once

/*
    Copyright (c) Tiger's Eye Jade Swiftwing, all rights reserved.
    This file is written by Tiger's Eye Jade Swiftwing.  It is licensed under the
GPLv3 license.  Note that my first name is "Tiger's Eye" (which is two words), my
middle name is "Jade", and "Swiftwing" is one word that is my last name.
    This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.  This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
details.  You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>.
*/

static inline __attribute__((always_inline, hot))
u64 RGBA_red(u64 size_x, u64 x, u64 y)
{
    return (((y * size_x) + x) * 4);
}
static inline __attribute__((always_inline, hot))
u64 RGBA_green(u64 size_x, u64 x, u64 y)
{
    return (((y * size_x) + x) * 4) + 1;
}
static inline __attribute__((always_inline, hot))
u64 RGBA_blue(u64 size_x, u64 x, u64 y)
{
    return (((y * size_x) + x) * 4) + 2;
}
static inline __attribute__((always_inline, hot))
u64 RGBA_alpha(u64 size_x, u64 x, u64 y)
{
    return (((y * size_x) + x) * 4) + 3;
}

static inline __attribute__((always_inline, hot))
u64 RGB_red(u64 size_x, u64 x, u64 y)
{
    return (((y * size_x) + x) * 3);
}
static inline __attribute__((always_inline, hot))
u64 RGB_green(u64 size_x, u64 x, u64 y)
{
    return (((y * size_x) + x) * 3) + 1;
}
static inline __attribute__((always_inline, hot))
u64 RGB_blue(u64 size_x, u64 x, u64 y)
{
    return (((y * size_x) + x) * 3) + 2;
}
