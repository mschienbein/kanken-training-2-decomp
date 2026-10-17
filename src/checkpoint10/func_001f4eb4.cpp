#include <stddef.h>
#include "game/functions.h"

struct W15iSoundPool;
struct W15iStreamItem;
struct W15iAddressCache {
    unsigned address;
    unsigned length;
    unsigned char active;
};
struct W15iRecordCache {
    unsigned words[3];
    unsigned char active;
};
struct W15iStreamVoice {
    unsigned char reserved00[0x1c];
    unsigned address;
};
struct W15iStreamSound;
struct W15iStreamVtable {
    unsigned char reserved00[0x0c];
    void (*stop)(W15iStreamSound *sound);
};
struct W15iStreamSound {
    const W15iStreamVtable *vtable;
    unsigned char reserved04[8];
    unsigned char registered;
    unsigned char flag0d;
    unsigned char flag0e;
    unsigned char stopping;
    unsigned char reserved10[0x34];
    RegistryEntry entry;
    unsigned char reserved50[0x40];
    W15iStreamVoice *voices[16];
    unsigned char reservedd0[0x24];
    W15iRecordCache records[4];
    W15iAddressCache caches[4];
    unsigned char reserved164;
    unsigned char event_active;
    unsigned char reserved166[0x0e];
    W15iSoundPool *pool;
    W15iStreamItem *item;
};

/* INTEGRATOR: move to include/game/stream_player.h */
extern "C" {
extern void func_001d3c58(W15iAddressCache *cache);
}

static inline W15iStreamVoice *voice_at(const W15iStreamSound *sound, int index)
{
    if (index > 15)
        return 0;
    return sound->voices[index];
}

extern "C" void func_001f4eb4(RegistryEntry *entry, unsigned begin, unsigned end)
{
    W15iStreamSound *sound = reinterpret_cast<W15iStreamSound *>(
        reinterpret_cast<char *>(entry) - offsetof(W15iStreamSound, entry));
    if (sound->registered) {
    for (int i = 0; i < 16; ++i) {
        W15iStreamVoice *voice = voice_at(sound, i);
        if (voice && begin <= voice->address && voice->address <= end) {
            sound->vtable->stop(sound);
            break;
        }
    }
    for (unsigned i = 0; i < 4; ++i) {
        unsigned address = sound->caches[i].address;
        if (begin <= address) {
            if (address <= end)
                func_001d3c58(&sound->caches[i]);
        }
    }
    }
}
