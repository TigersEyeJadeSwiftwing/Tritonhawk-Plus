#pragma once

/* s_isnanl.c -- long double version of s_isnan.c.
 * Conversion to long double by Jakub Jelinek, jj@ultra.linux.cz.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * isnanq(x) returns 1 is x is nan, else 0;
 * no branching!
 */

// #include "quadmath-imp.h"

static inline __attribute__((always_inline, hot))
int isnanq(__float128 x)
{
    int64_t hx,lx;
    GET_FLT128_WORDS64(hx,lx,x);
    hx &= 0x7fffffffffffffffLL;
    hx |= (uint64_t)(lx|(-lx))>>63;
    hx = 0x7fff000000000000LL - hx;
    return (int)((uint64_t)hx>>63);
}
