/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_MmlSequenceTrackAllocator.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_MML_SEQUENCE_TRACK_ALLOCATOR_H_
#define NW_SND_MML_SEQUENCE_TRACK_ALLOCATOR_H_

#include "snd_SequenceTrackAllocator.h"
#include "snd_MmlSequenceTrack.h"
#include "snd_InstancePool.h"

namespace nw { 
namespace snd { 
namespace internal { 
namespace driver {

class MmlParser;

class MmlSequenceTrackAllocator : public SequenceTrackAllocator
{
    typedef InstancePool<MmlSequenceTrack> MmlSequenceTrackPool;

public:
    explicit MmlSequenceTrackAllocator(MmlParser* parser) : m_pParser(parser) {}
    void SetMmlParser(MmlParser* parser) { m_pParser = parser; }
    MmlParser* GetMmlParser() { return m_pParser; }
    virtual SequenceTrack* AllocTrack(SequenceSoundPlayer* player);
    virtual void FreeTrack(SequenceTrack* track);
    virtual int GetAllocatableTrackCount() const { return m_TrackPool.Count(); }

    unsigned long Create(void* buffer, unsigned long size);
    void Destroy(void* buffer, unsigned long size);

private:
    MmlParser* m_pParser;
    MmlSequenceTrackPool m_TrackPool;
};

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_MML_SEQUENCE_TRACK_ALLOCATOR_H_

