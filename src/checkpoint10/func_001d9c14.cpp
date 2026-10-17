#include "game/sound_runtime.h"
#include "game/functions.h"


void func_001d9c14(WaveSoundControl *sound)
{
    if (sound->active) {
        sound->active = 0;
        sound->enabled = 0;
        func_0016a3e0(&sound->sound);
        SoundInstancePool *pool = sound->pool;
        func_0018d868(&pool->active, &sound->sound.pool_link);
        sound->sound.vtable->stop(&sound->sound);
        PoolList *available = &pool->available;
        PoolLink *position = &available->sentinel;
        PoolLink *link = &sound->sound.pool_link;
        func_00191154(available, position, link);
    }
}
