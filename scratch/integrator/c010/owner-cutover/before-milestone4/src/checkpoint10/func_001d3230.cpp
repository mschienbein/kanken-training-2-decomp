#include "game/sound_runtime.h"
#include "game/functions.h"

struct SequencePoolSound {
    SoundInstance sound;
    unsigned char reserved118[0x2314 - 0x118];
    RegistryEntry entry;
    unsigned char reserved2320[0x3044 - 0x2320];
    SoundInstancePool *pool;
    unsigned char reserved3048[0x328e - 0x3048];
    bool active;
    bool isActive() const { return active; }
    void setInactive() { active = 0; }
};

/* INTEGRATOR: move SequencePoolSound and its declarations to the shared header
 * once task-cancellation ownership is reconciled. This caller supplies the
 * embedded entry address, while the accepted wrapper still uses a scalar. */
extern "C" {
void func_001d6b24(RegistryEntry *entry);
void func_001d3230(SequencePoolSound *sound);
}

void func_001d3230(SequencePoolSound *sound)
{
    if (sound->isActive()) {
        sound->setInactive();
        func_001d6b24(&sound->entry);
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
