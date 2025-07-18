#pragma once

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
    #include "truncq.inl"
    #include "clampq.inl"
    #include "powq.inl"
    #include "distwarpq.inl"
    #include "fabsq.inl"
#else
    #include "clampq.inl"
    #include "distwarpq.inl"
#endif

/*
Copyright (c) Tiger's Eye Jade Swiftwing, all rights reserved.

This file is written by Tiger's Eye Jade Swiftwing.  It is licensed under the GPLv3 license.
Note that my first name is "Tiger's Eye" (which is two words), my middle name is "Jade", and "Swiftwing" is one word that is my last name.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef THP_USING_LONG_DOUBLE_FOR_128_BIT_FLOAT
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
    __float128 factor0 = (clamped < 0.5q) ?
        (0.5q - clamped) * 2.0q :
        (clamped - 0.5q) * 2.0q ;
    __float128 factor1 = powq(factor0, 1.0q / exponent);
    __float128 lerp_fac = (clamped < 0.5q) ?
        0.5q - (factor1 * 0.5q) :
        0.5q + (factor1 * 0.5q) ;
    return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
__float128 lerpwarpq(__float128 a, __float128 b, __float128 lerp_factor, __float128 dist_warp_factor)
{
    __float128 clamped = clamp01q(lerp_factor);
    __float128 lerp_fac = 1.0q - fabsq(distwarpq((1.0q - clamped) * dist_warp_factor));
    return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
__float128 lerp2warpq(__float128 a, __float128 b, __float128 lerp_factor, __float128 dist_warp_factor)
{
    __float128 clamped = clamp01q(lerp_factor);
    __float128 factor0 = (clamped < 0.5q) ?
        (0.5q - clamped) * 2.0q :
        (clamped - 0.5q) * 2.0q ;
    __float128 factor1 = distwarpq(factor0 * dist_warp_factor);
    __float128 lerp_fac = (clamped < 0.5q) ?
        0.5q - (factor1 * 0.5q) :
        0.5q + (factor1 * 0.5q) ;
    return (a * (1.0q - lerp_fac)) + (b * lerp_fac);
}
#else
static inline __attribute__((always_inline, hot))
long double lerpq(long double a, long double b, long double lerp_factor)
{
    long double lerp_fac = clamp01q(lerp_factor);
    return (a * (1.0L - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
long double lerpexpq(long double a, long double b, long double lerp_factor, long double exponent)
{
    long double lerp_fac = powq(clamp01q(lerp_factor), exponent);
    return (a * (1.0L - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
long double lerp2expq(long double a, long double b, long double lerp_factor, long double exponent)
{
    long double clamped = clamp01q(lerp_factor);
    long double factor0 = (clamped < 0.5L) ?
        (0.5L - clamped) * 2.0L :
        (clamped - 0.5L) * 2.0L ;
    long double factor1 = powq(factor0, 1.0L / exponent);
    long double lerp_fac = (clamped < 0.5L) ?
        0.5L - (factor1 * 0.5L) :
        0.5L + (factor1 * 0.5L) ;
    return (a * (1.0L - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
long double lerpwarpq(long double a, long double b, long double lerp_factor, long double dist_warp_factor)
{
    long double clamped = clamp01q(lerp_factor);
    long double lerp_fac = 1.0L - fabsq(distwarpq((1.0L - clamped) * dist_warp_factor));
    return (a * (1.0L - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
long double lerp2warpq(long double a, long double b, long double lerp_factor, long double dist_warp_factor)
{
    long double clamped = clamp01q(lerp_factor);
    long double factor0 = (clamped < 0.5L) ?
        (0.5L - clamped) * 2.0L :
        (clamped - 0.5L) * 2.0L ;
    long double factor1 = distwarpq(factor0 * dist_warp_factor);
    long double lerp_fac = (clamped < 0.5L) ?
        0.5L - (factor1 * 0.5L) :
        0.5L + (factor1 * 0.5L) ;
    return (a * (1.0L - lerp_fac)) + (b * lerp_fac);
}
#endif

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
    float factor0 = (clamped < 0.5f) ?
        (0.5f - clamped) * 2.0f :
        (clamped - 0.5f) * 2.0f ;
    float factor1 = powf(factor0, 1.0f / exponent);
    float lerp_fac = (clamped < 0.5f) ?
        0.5f - (factor1 * 0.5f) :
        0.5f + (factor1 * 0.5f) ;
    return (a * (1.0f - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
float lerpwarpf(float a, float b, float lerp_factor, float dist_warp_factor)
{
    float clamped = clamp01f(lerp_factor);
    float lerp_fac = 1.0f - fabsf(distwarpf((1.0f - clamped) * dist_warp_factor));
    return (a * (1.0f - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
float lerp2warpf(float a, float b, float lerp_factor, float dist_warp_factor)
{
    float clamped = clamp01f(lerp_factor);
    float factor0 = (clamped < 0.5f) ?
        (0.5f - clamped) * 2.0f :
        (clamped - 0.5f) * 2.0f ;
    float factor1 = distwarpf(factor0 * dist_warp_factor);
    float lerp_fac = (clamped < 0.5f) ?
        0.5f - (factor1 * 0.5f) :
        0.5f + (factor1 * 0.5f) ;
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
    double factor0 = (clamped < 0.5) ?
        (0.5 - clamped) * 2.0 :
        (clamped - 0.5) * 2.0 ;
    double factor1 = pow(factor0, 1.0 / exponent);
    double lerp_fac = (clamped < 0.5) ?
        0.5 - (factor1 * 0.5) :
        0.5 + (factor1 * 0.5) ;
    return (a * (1.0 - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
double lerpwarpd(double a, double b, double lerp_factor, double dist_warp_factor)
{
    double clamped = clamp01d(lerp_factor);
    double lerp_fac = 1.0 - fabs(distwarpd((1.0 - clamped) * dist_warp_factor));
    return (a * (1.0 - lerp_fac)) + (b * lerp_fac);
}
static inline __attribute__((always_inline, hot))
double lerp2warpd(double a, double b, double lerp_factor, double dist_warp_factor)
{
    double clamped = clamp01d(lerp_factor);
    double factor0 = (clamped < 0.5) ?
        (0.5 - clamped) * 2.0 :
        (clamped - 0.5) * 2.0 ;
    double factor1 = distwarpd(factor0 * dist_warp_factor);
    double lerp_fac = (clamped < 0.5) ?
        0.5 - (factor1 * 0.5) :
        0.5 + (factor1 * 0.5) ;
    return (a * (1.0 - lerp_fac)) + (b * lerp_fac);
}
