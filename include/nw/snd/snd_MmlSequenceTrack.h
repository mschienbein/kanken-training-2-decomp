/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_MmlSequenceTrack.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_MML_SEQUENCE_TRACK_H_
#define NW_SND_MML_SEQUENCE_TRACK_H_

#include "snd_SequenceTrack.h"

namespace nw { 
namespace snd { 
namespace internal { 
namespace driver {

class MmlParser;

class MmlSequenceTrack : public SequenceTrack
{
public:
    MmlSequenceTrack();

    void SetMmlParser(const MmlParser* parser) { m_pParser = parser; }
    const MmlParser* GetMmlParser() const { return m_pParser; }

protected:
    virtual SequenceTrack::ParseResult Parse(bool doNoteOn);
    
private:
    const MmlParser* m_pParser;
};

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_MML_SEQUENCE_TRACK_H_

