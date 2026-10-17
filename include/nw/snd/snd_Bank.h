/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_Bank.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_BANK_H_
#define NW_SND_BANK_H_

#include "snd_NoteOnCallback.h"
#include "snd_Channel.h"
#include "snd_PlayerHeapDataManager.h"

namespace nw {
namespace snd {

class SoundArchive;
class SoundArchivePlayer;

namespace internal {
namespace driver {

struct Bank
{
    Bank()
    {
    }
    ~Bank()
    {
    }
    Channel* NoteOn(const void* bankFile, const NoteOnInfo& noteOnInfo, const SoundArchive& archive,
        const SoundArchivePlayer& player, const PlayerHeapDataManager* dataMgr) const;
};

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw


#endif // NW_SND_BANK_H_

