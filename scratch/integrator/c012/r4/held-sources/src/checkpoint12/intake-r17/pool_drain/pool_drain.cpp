#include <stddef.h>
#include "game/scene_lifecycle.h"
#include "game/audio_tasks.h"


static AudioVoiceGroup *voice_from_link(PoolLink *link)
{
    return reinterpret_cast<AudioVoiceGroup *>(
        reinterpret_cast<char *>(link) - offsetof(AudioVoiceGroup, link));
}


void func_00131464(TaskPool *pool)
{
    if (pool->initialized) {
        for (;;) {
            if (pool->active.count == 0)
                break;
            AudioVoiceGroup *voice = voice_from_link(pool->active.sentinel.next);
            func_001835b8(voice);
            if (voice->callback != 0)
                voice->callback(voice, 1, voice->context);
            func_00183550(voice);
        }
        for (;;) {
            if (pool->available.count == 0)
                break;
            AudioVoiceGroup *voice = voice_from_link(pool->available.sentinel.next);
            func_00191178(&pool->available, &voice->link);
            func_00138cb0(voice);
        }
        pool->initialized = 0;
    }
}
