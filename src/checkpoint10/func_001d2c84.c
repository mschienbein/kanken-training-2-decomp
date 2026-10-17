#include "game/pool_list.h"

struct SoundInstance;
struct SoundPlayer;
struct SoundHandle {
    struct SoundInstance *sound;
};
struct SoundInstanceVTable {
    unsigned char reserved00[0x10];
    void (*stop)(struct SoundInstance *sound);
    unsigned char reserved14[0x10];
    void (*priority_changed)(struct SoundInstance *sound);
};

/* INTEGRATOR: differs from include/game/sound_runtime.h: the pointer at 0x10
 * is a notification receiver; vtable slot 0x24 receives the priority update. */
struct SoundInstance {
    const struct SoundInstanceVTable *vtable;
    unsigned char reserved04[4];
    struct SoundHandle *handle;
    struct SoundHandle *secondary_handle;
    struct SoundPlayer *player;
    unsigned char reserved14[0x3c];
    int priority_offset;
    unsigned char reserved54[0x49];
    unsigned char priority;
    unsigned char reserved9e[0x2a];
    float volume;
    unsigned char reservedcc[0x3c];
    struct PoolLink player_link;
    struct PoolLink selection_link;
};

extern void func_00143328(struct SoundPlayer *player, struct SoundInstance *sound);

void func_001d2c84(struct SoundInstance *sound, unsigned char priority)
{
    sound->priority = priority;
    if (sound->player != 0)
        func_00143328(sound->player, sound);
    sound->vtable->priority_changed(sound);
}
