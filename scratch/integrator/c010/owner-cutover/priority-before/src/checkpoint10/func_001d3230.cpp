#include "game/sound_runtime.h"
#include "game/functions.h"

/* The frozen SoundInstance view hides the priority-pool link at 0xf8.  Keep
 * the canonical object as the union's principal view until that field is named. */
struct SoundPoolLinkage {
    unsigned char reserved00[0xf8];
    PoolLink pool_link;
};
union SoundPoolObject {
    SoundInstance instance;
    SoundPoolLinkage linkage;
};
struct SoundInstancePool {
    unsigned char reserved00[8];
    PoolList active;
    PoolList available;
};
struct SequencePoolSound {
    SoundPoolObject sound;
    unsigned char reserved118[0x2314 - 0x118];
    RegistryEntry entry;
    unsigned char reserved2320[0x3044 - 0x2320];
    SoundInstancePool *pool;
    unsigned char reserved3048[0x328e - 0x3048];
    bool active;
    bool isActive() const { return active; }
    void setInactive() { active = 0; }
};

/* INTEGRATOR: move to include/game/sound_runtime.h; name pool_link at 0xf8.
 * func_001d6b24 differs from src/wave11/func_001d6b24.c: this caller supplies
 * the embedded entry address, not a scalar index.  Its pointed-to role remains
 * [INFERENCE] until the task-cancellation contract is unified. */
extern "C" {
void func_0016a3e0(SoundInstance *sound);
void func_001d6b24(RegistryEntry *entry);
void func_001d3230(SequencePoolSound *sound);
}

void func_001d3230(SequencePoolSound *sound)
{
    if (sound->isActive()) {
        sound->setInactive();
        func_001d6b24(&sound->entry);
        func_0016a3e0(&sound->sound.instance);
        SoundInstancePool *pool = sound->pool;
        func_0018d868(&pool->active, &sound->sound.linkage.pool_link);
        sound->sound.instance.vtable->stop(&sound->sound.instance);
        PoolList *available = &pool->available;
        PoolLink *position = &available->sentinel;
        PoolLink *link = &sound->sound.linkage.pool_link;
        func_00191154(available, position, link);
    }
}
