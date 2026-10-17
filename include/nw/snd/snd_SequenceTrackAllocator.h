/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_SequenceTrackAllocator.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_SEQUENCE_TRACK_ALLOCATOR_H_
#define NW_SND_SEQUENCE_TRACK_ALLOCATOR_H_

#include "snd_Global.h"

namespace nw { 
namespace snd { 
namespace internal { 
namespace driver {

class SequenceTrack;
class SequenceSoundPlayer;

class SequenceTrackAllocator
{
public:
    virtual ~SequenceTrackAllocator() {}

    virtual SequenceTrack* AllocTrack(SequenceSoundPlayer* player) = 0;
    virtual void FreeTrack(SequenceTrack* track) = 0;

    virtual int GetAllocatableTrackCount() const = 0;
};

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_SEQUENCE_TRACK_ALLOCATOR_H_

