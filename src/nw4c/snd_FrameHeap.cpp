/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_FrameHeap.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select the new constructor; the SaveState method collected in c006 remains declaration-only. Omit its unused inline helper. Retained algorithm unchanged.
 * This file is free software under GNU GPL version 3; see LICENSE.txt.
 */
#define NW_RELEASE
#include <nw/snd/snd_FrameHeap.h>

namespace nw { namespace snd { namespace internal {
FrameHeap::FrameHeap(): 
    m_pHeap(NULL) 
{
}


} } } 
