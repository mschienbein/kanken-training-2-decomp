#include "game/functions.h"

struct SoundInstance;
struct SoundHandle { SoundInstance *sound; };
struct SoundInstanceVTable {
    unsigned char reserved00[0x10];
    void (*stop)(SoundInstance *sound);
};
/* INTEGRATOR: differs from include/game/sound_runtime.h: expose the pool link
 * at 0xf8 inside reservedcc without moving the two existing links. */
struct SoundInstance {
    const SoundInstanceVTable *vtable;
    unsigned char reserved04[4];
    SoundHandle *handle;
    SoundHandle *secondary_handle;
    unsigned char reserved10[0x40];
    int priority_offset;
    unsigned char reserved54[0x49];
    unsigned char priority;
    unsigned char reserved9e[0x2a];
    float volume;
    unsigned char reservedcc[0x2c];
    PoolLink pool_link;
    unsigned char reserved100[8];
    PoolLink player_link;
    PoolLink selection_link;
};

struct SoundInstancePool {
    unsigned char reserved00[8];
    PoolList active;
    PoolList available;
};
struct SequenceSoundControl {
    SoundInstance sound;
    void *source;
    SoundInstancePool *pool;
    unsigned char active;
    unsigned char enabled;
};

/* INTEGRATOR: move to include/game/sound_runtime.h */
extern "C" {
void func_0016a3e0(SoundInstance *sound);
void func_001d3bcc(SequenceSoundControl *sound);
}

void func_001d3bcc(SequenceSoundControl *sound)
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
