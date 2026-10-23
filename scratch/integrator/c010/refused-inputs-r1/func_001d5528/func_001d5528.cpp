#include <preinclude.h>
#include "game/audio_voice.h"
#include "nw/snd/snd_InstancePool.h"
#include "nw/ut/ut_LinkList.h"
#include <stddef.h>

/* [INFERENCE] This is the NW4C driver pool allocation (InstancePool<Channel> plus the
 * active-channel LinkList, i.e. ChannelManager::Alloc); include/nw/snd/snd_Channel.h
 * does not compile standalone, so the pooled object is the shared AudioVoice layout
 * (0x1b0 bytes, `next` at 0x1ac) followed by Channel::m_Link at 0x1b0. The out-of-line
 * constructor is the callee func_001d9820 (needs_names: _ZN16PooledAudioVoiceC1Ev). */
struct PooledAudioVoice : AudioVoice {
    PooledAudioVoice();
    nw::ut::LinkListNode link;
};

struct ObjectPool {
    nw::snd::internal::InstancePool<PooledAudioVoice> free_list;
    nw::ut::LinkList<PooledAudioVoice, offsetof(PooledAudioVoice, link)> active;
    unsigned char initialized;
    unsigned char reserved11[3];
    unsigned capacity;
    void *storage;
    unsigned storage_size;
};

extern "C" AudioVoice *func_001d5528(ObjectPool *pool)
{
    PooledAudioVoice *voice = pool->free_list.Alloc();
    /* ORIGINAL BUG: exhausted allocation still forms the member link of NULL. */
    pool->active.PushBack(voice);
    return voice;
}
