/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/types.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain public layouts and inline definitions; normalize types/assertion include paths for standalone release compilation.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#ifndef NULL
#define NULL (void*)0
#endif

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef unsigned short ushort;
typedef unsigned int uint;

typedef signed char        s8;
typedef signed short       s16;
typedef signed int         s32;
typedef signed long long   s64;

typedef u32 uptr;
typedef s32 ptr;

typedef float              f32;
typedef double             f64;

typedef volatile u8       vu8;
typedef volatile u16      vu16;
typedef volatile u32      vu32;
typedef volatile u64      vu64;

typedef volatile s8       vs8;
typedef volatile s16      vs16;
typedef volatile s32      vs32;
typedef volatile s64      vs64;

typedef volatile f32      vf32;
typedef volatile f64      vf64;

typedef unsigned char           bit8;
typedef unsigned short          bit16;
typedef unsigned int            bit32;
typedef unsigned long long int  bit64;

typedef unsigned int SigWord;

namespace nw
{
typedef unsigned long IntPtr;
typedef signed long PtrDiff;
}

#ifndef nullptr
#define nullptr NULL
#endif

#ifndef NW_ANY_TO_PTR_VALUE
#define NW_ANY_TO_PTR_VALUE(ptr) reinterpret_cast<nw::IntPtr>(reinterpret_cast<const void*>(ptr))
#endif

#define NW_INLINE inline

#ifndef NW_RELEASE
#define NW_FORCE_INLINE inline
#else
#define NW_FORCE_INLINE __forceinline
#endif

#define NW_UNUSED_VARIABLE(x) ((void)(x))

#define NW_ALIGN(x) __attribute__((aligned(x)))

#define NW_ROUND_UP_32B(x) (((u32)(x) + 32 - 1) & ~(32 - 1))

