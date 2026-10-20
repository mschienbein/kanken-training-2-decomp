/* SPDX-License-Identifier: GPL-3.0-only
 * Uses pinned NintendoWare4Ctr snd_Util.h reference-table implementation.
 * [INFERENCE] The prefetch data block uses the native sound-file reference table.
 */
#include "game/prefetch_stream.h"

const PrefetchFileBlock *func_00153f28(const PrefetchFileHeader *file)
{
    return static_cast<const PrefetchFileBlock *>(file->GetBlock(0x4004));
}
