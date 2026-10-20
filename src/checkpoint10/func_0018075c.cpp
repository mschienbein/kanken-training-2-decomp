#include "game/sound_runtime.h"



static inline unsigned CreateState(FreeListPool *state, void *buffer, unsigned size, unsigned stride)
{
    return func_00180768(state, buffer, size, stride);
}
extern "C" unsigned func_0018075c(SoundPool *pool, void *buffer, unsigned size)
{
    return CreateState(&pool->state, buffer, size, 0x138);
}
