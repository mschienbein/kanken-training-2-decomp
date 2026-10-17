/* SPDX-License-Identifier: GPL-3.0-or-later
 * Project-owned declarations based only on public NW4C uses at upstream commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd. No Nintendo SDK headers copied.
 * MEM_VRAMA's ordinal is inferred from public NW4C's first VRAM selector.
 */
#pragma once
#include "gx/CTR/gx_CommandAccess.h"
namespace nn { namespace gx { namespace CTR {
enum MemoryArea { MEM_VRAMA = 0, MEM_VRAMB = 1 };
uptr GetVramStartAddr(MemoryArea area);
} } }
extern "C" uptr nngxGetPhysicalAddr(uptr address);
