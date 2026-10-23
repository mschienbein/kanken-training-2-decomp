/* SPDX-License-Identifier: GPL-3.0-only
 * Iterator layering adapted from NintendoWare4Ctr contributors:
 * https://github.com/ctrdecomp/NintendoWare4Ctr/tree/15ddb5632db5a6f0d2f46e3827fc78df468684bd
 * sources/libraries/snd/snd_ChannelManager.cpp and include/nw/ut/ut_LinkList.h.
 * Modified 2026-10-17: canonical project types and explicit external call binding.
 * GPL version 3; see src/nw4c/LICENSE.txt and THIRD_PARTY.md.
 */
#include <stddef.h>
#include "game/audio_voice.h"
#include "game/object_pool.h"
#include "game/sound.h"

/* [INFERENCE] The 432-byte payload and following intrusive link agree with the
 * channel-manager iteration; descriptive project type names are retained. */



class VoiceIterator {
    PoolIterator iterator;
public:
    explicit VoiceIterator(PoolLink *p) : iterator(p) {}
    AudioVoice *operator->() const {
        return reinterpret_cast<AudioVoice *>(
            reinterpret_cast<char *>(iterator.get()) - offsetof(AudioVoice, link));
    }
    VoiceIterator &operator++() { ++iterator; return *this; }
    VoiceIterator operator++(int) { const VoiceIterator old(*this); ++*this; return old; }
    friend bool operator==(VoiceIterator a, VoiceIterator b) { return a.iterator == b.iterator; }
    friend bool operator!=(VoiceIterator a, VoiceIterator b) { return !(a == b); }
};

static VoiceIterator voices_begin(ObjectPool *pool)
{
    return VoiceIterator(pool->active.sentinel.next);
}

static VoiceIterator voices_end(ObjectPool *pool)
{
    return VoiceIterator(&pool->active.sentinel);
}


void func_0014d308(ObjectPool *pool)
{
    for (VoiceIterator it = voices_begin(pool); it != voices_end(pool); ) {
        VoiceIterator current = it++;
        func_0014f304(current.operator->(), true);
    }
}
