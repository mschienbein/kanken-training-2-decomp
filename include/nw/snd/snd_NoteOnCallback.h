/* SPDX-License-Identifier: GPL-3.0-only
 * NintendoWare4Ctr contributors; upstream include/nw/snd/snd_NoteOnCallback.h
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Pinned commit: 15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * Modified 2026-10-17: standalone UTF-8 and relative include normalization.
 * GNU GPL version 3; see LICENSE.txt.
 */
#ifndef NW_SND_NOTE_ON_CALLBACK_H_
#define NW_SND_NOTE_ON_CALLBACK_H_

#include "snd_Channel.h"

namespace nw { 
namespace snd { 
namespace internal { 
namespace driver {

class SequenceSoundPlayer;

struct NoteOnInfo
{
    int prgNo;
    int key;
    int velocity;
    int length;
    int initPan;
    int priority;
    Channel::ChannelCallback channelCallback;
    u32 channelCallbackData;
};


class NoteOnCallback
{
public:
    virtual ~NoteOnCallback() {}

    virtual Channel* NoteOn(SequenceSoundPlayer* sequenceSoundPlayer, u8 bankIndex, const NoteOnInfo& noteOnInfo) = 0;
};

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

#endif // NW_SND_NOTE_ON_CALLBACK_H_

