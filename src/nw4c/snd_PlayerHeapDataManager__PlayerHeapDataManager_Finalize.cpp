/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream sources/libraries/snd/snd_PlayerHeapDataManager.cpp
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: select located C++ definitions and used inline helpers; omit other definitions and assembly alternatives; normalize relative includes/encoding; algorithms/constants unchanged
 * GNU GPL version 3; see LICENSE.txt.
 */
#include <preinclude.h>
// Filename: snd_PlayerHeapDataManager.cpp
//
// Project: NintendoWare4Ctr

#include <nw/snd/snd_PlayerHeapDataManager.h>

namespace nw {
namespace snd {
namespace internal {

void PlayerHeapDataManager::Finalize()
{
    if (m_IsFinalized)
    {
        return;
    }
    m_IsInitialized = false;
    m_IsFinalized = true;
    SetSoundArchive(NULL);
}

} // namespace internal
} // namespace snd
} // namespace nw

