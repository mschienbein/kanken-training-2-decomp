#ifndef GAME_SOUND_CACHE_REGISTRATION_H
#define GAME_SOUND_CACHE_REGISTRATION_H
#include "game/functions.h"

struct SoundResourceCache;
#ifdef __cplusplus
extern "C" {
#endif
extern struct SoundResourceCache bss_00246ae8;
extern volatile unsigned data_0023ecec;
extern struct SoundResourceCache *func_001b4f58(struct SoundResourceCache *cache);
extern void func_001b501c(struct SoundResourceCache *cache);
/* Erased registration: this exact destructor type is neither stored nor invoked. */
extern void register_sound_resource_cache_cleanup(struct SoundResourceCache *cache,
    void (*destroy)(struct SoundResourceCache *), void (*image_base)(void));
#ifdef __cplusplus
}
#endif
#endif
