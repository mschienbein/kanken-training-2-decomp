/* SPDX-License-Identifier: GPL-3.0-or-later
 * Project-owned declarations from NW4C include/nw/os/os_Memory.h, pinned commit
 * 15ddb5632db5a6f0d2f46e3827fc78df468684bd; no SDK header or implementation. */
#pragma once
#include "../nw/types.h"
namespace nn { namespace nstd { namespace ARMv6 {
void MemCpy(void* destination, const void* source, size_t size);
} } }
