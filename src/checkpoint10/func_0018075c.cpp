struct SoundPoolLink;
/* INTEGRATOR: move to include/game/sound_runtime.h */
extern "C" {
unsigned func_00180768(SoundPoolLink **free_list, void *buffer, unsigned size, unsigned stride);
}

struct SoundPoolLink {
    SoundPoolLink *next;
};

struct SoundPoolState {
    SoundPoolLink *free;
};

struct SoundPool {
    unsigned char reserved00[8];
    SoundPoolState state;
};



static inline unsigned CreateState(SoundPoolState *state, void *buffer, unsigned size, unsigned stride)
{
    return func_00180768(&state->free, buffer, size, stride);
}
extern "C" unsigned func_0018075c(SoundPool *pool, void *buffer, unsigned size)
{
    return CreateState(&pool->state, buffer, size, 0x138);
}
