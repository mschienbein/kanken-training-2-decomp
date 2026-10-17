/* SPDX-License-Identifier: GPL-3.0-only
 * Uses pinned NintendoWare4Ctr snd_Util.h reference-table implementation.
 * [INFERENCE] The prefetch data block uses the native sound-file reference table.
 */
#define NW_RELEASE
#include <nw/snd/snd_Util.h>

struct PrefetchFileBlock;
/* INTEGRATOR: differs from src/wave10/func_001528dc.c: the serialized header's
 * fields/12-byte references agree with native Util::SoundFileHeader; unify the
 * C header view and this native C++ view in a shared declaration before adoption. */
/* INTEGRATOR: move to include/game/prefetch_stream.h */
extern "C" {
const PrefetchFileBlock *func_00153f28(
    const nw::snd::internal::Util::SoundFileHeader *file);
}
const PrefetchFileBlock *func_00153f28(
    const nw::snd::internal::Util::SoundFileHeader *file)
{
    return static_cast<const PrefetchFileBlock *>(file->GetBlock(0x4004));
}
