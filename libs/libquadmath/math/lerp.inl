#pragma once

#include "truncq.inl"
#include "clampq.inl"
#include "powq.inl"

/*
This file is written by Tiger's Eye Jade Swiftwing, for part of a custom quadmath 128-bit floating type math library.
Note that my first name is "Tiger's Eye" (which is two words), my middle name is "Jade", and "Swiftwing" is one word that is my last name.
It is released under a zlib license, so feel free to use it, modify it, whatever, under the terms of the zlib license.

Copyright (c) 2025 Tiger's Eye Jade Swiftwing

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
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
