/* SPDX-License-Identifier: GPL-3.0-or-later
 * Project-owned declarations from public NW4C call sites, pinned upstream
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd; no Nintendo SDK source.
 * nngxUpdateBuffer keeps the current project's int-return ABI declaration.
 */
#pragma once
#include "../../../nw/types.h"
#include "gx_PicaAll.h"
extern "C" int nngxUpdateBuffer(const void* buffer, u32 size);
