/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/ut/ut_Preprocessor.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: retain public layouts and inline definitions; normalize types/assertion include paths for standalone release compilation.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#pragma once

#define NW_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);                \
    void operator=(const TypeName&)

