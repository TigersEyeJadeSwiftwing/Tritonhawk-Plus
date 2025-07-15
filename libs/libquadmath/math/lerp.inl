#pragma once

#include "truncq.inl"
#include "clampq.inl"
#include "powq.inl"

/*
Copyright (c) Tiger's Eye Jade Swiftwing, all rights reserved.

This file is written by Tiger's Eye Jade Swiftwing.  It is licensed under the GPLv3 license.
Note that my first name is "Tiger's Eye" (which is two words), my middle name is "Jade", and "Swiftwing" is one word that is my last name.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

static inline __attribute__((always_inline, hot))
__float128 lerpq(__float128 a, __float128 b, __float128 lerp_factor)
{
    __float128 lerp_fac = clamp01q(lerp_factor);
    return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
__float128 lerpexpq(__float128 a, __float128 b, __float128 lerp_factor, __float128 exponent)
{
    __float128 lerp_fac = powq(clamp01q(lerp_factor), exponent);
    return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
__float128 lerp2expq(__float128 a, __float128 b, __float128 lerp_factor, __float128 exponent)
{
    __float128 clamped = clamp01q(lerp_factor);
    __float128 lerp_fac = (clamped >= 0.5q) ? (powq(clamped * 2.q, exponent) * 2.q) : (1.q - (powq((1.q - clamped) * 2.q, exponent) * 2.q));
    return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
}

static inline __attribute__((always_inline, hot))
float lerpf(float a, float b, float lerp_factor)
{
    return (a * (1.0f - lerp_factor)) + (b * lerp_factor);
}
static inline __attribute__((always_inline, hot))
float lerpexpf(float a, float b, float lerp_factor, float exponent)
{
    float lerp_fac = powf(clamp01f(lerp_factor), exponent);
    return (a * (1.0f - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
float lerp2expf(float a, float b, float lerp_factor, float exponent)
{
    float clamped = clamp01f(lerp_factor);
    float lerp_fac = (clamped >= 0.5f) ? (powf(clamped * 2.f, exponent) * 2.f) : (1.f - (powf((1.f - clamped) * 2.f, exponent) * 2.f));
    return (a * (1.0f - lerp_fac)) + (b * lerp_fac);
}

static inline __attribute__((always_inline, hot))
double lerpd(double a, double b, double lerp_factor)
{
    return (a * (1.0 - lerp_factor)) + (b * lerp_factor);
}
static inline __attribute__((always_inline, hot))
double lerpexpd(double a, double b, double lerp_factor, double exponent)
{
    double lerp_fac = pow(clamp01d(lerp_factor), exponent);
    return (a * (1.0 - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
double lerp2expd(double a, double b, double lerp_factor, double exponent)
{
    double clamped = clamp01d(lerp_factor);
    double lerp_fac = (clamped >= 0.5) ? (pow(clamped * 2.0, exponent) * 2.0) : (1.0 - (pow((1.0 - clamped) * 2.0, exponent) * 2.0));
    return (a * (1.0 - lerp_fac)) + (b * lerp_fac);
}
