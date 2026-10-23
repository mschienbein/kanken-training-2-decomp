#include "game/sound_runtime.h"
#include "game/audio_tasks.h"
#include "game/functions.h"


void func_001d3230(SequencePoolSound *sound)
{
    if (sound->isActive()) {
        sound->setInactive();
        func_001d6b24((unsigned)sound->task_identity);
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
