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
__float128 fmodq(__float128 x, __float128 y)
{
    inline static constexpr __float128 one = 1.q, zero[] = {0.q, -0.q};
    int64_t hx, hy, sx;
    uint64_t lx, ly;

    GET_FLT128_WORDS64(hx, lx, x);
    GET_FLT128_WORDS64(hy, ly, y);
    sx = hx & 0x8000000000000000ULL; // sign of x

    if ((hy | ly) == 0 || (hx >= 0x7fff000000000000LL) || ((hy | ((ly - ly) >> 63)) > 0x7fff000000000000LL))
        return (x * y) / (x * y); // Handle special cases

    if (hx <= hy)
        return (hx < hy || lx < ly) ? x : zero[static_cast<uint64_t>(sx) >> 63]; // |x| < |y| or |x| = |y|

    int64_t exp_diff = ((hx >> 48) - 0x3fff) - ((hy >> 48) - 0x3fff); // Determine the exponent difference

    if (exp_diff >= 0)
    {
        hx = 0x0001000000000000LL | (0x0000ffffffffffffLL & hx); // Normalize x
    }
    else
    {
        int64_t shift = -exp_diff;
        if (shift <= 63)
        {
            hx = (hx << shift) | (lx >> (64 - shift));
            lx <<= shift;
        }
        else
        {
            hx = lx << (shift - 64);
            lx = 0;
        }
    }

    hy = 0x0001000000000000LL | (0x0000ffffffffffffLL & hy); // Normalize y

    while (exp_diff--)
    {
        if (hx < hy || (hx == hy && lx < ly))
        {
            hx = hx + hx + (lx >> 63);
            lx = lx + lx;
        }
        else
        {
            hx -= hy;
            lx -= ly;
        }
    }

    if ((hx | lx) == 0)
        return zero[static_cast<uint64_t>(sx) >> 63]; // Return sign(x) * 0

    while (hx < 0x0001000000000000LL)
    {
        hx = hx + hx + (lx >> 63);
        lx = lx + lx;
        exp_diff--;
    }

    if (exp_diff >= -16382)
    {
        hx = ((hx - 0x0001000000000000LL) | ((exp_diff + 16383) << 48)) | sx;
        SET_FLT128_WORDS64(x, hx, lx);
    }
    else
    {
        int64_t shift = -16382 - exp_diff;
        if (shift <= 48)
        {
            lx = (lx >> shift) | (static_cast<uint64_t>(hx) << (64 - shift));
            hx >>= shift;
        }
        else if (shift <= 63)
        {
            lx = (hx << (64 - shift)) | (lx >> shift);
            hx = sx;
        }
        else
        {
            lx = hx >> (shift - 64);
            hx = sx;
        }
        SET_FLT128_WORDS64(x, hx | sx, lx);
    }

    x *= one;

    return x;
}
