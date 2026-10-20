#include <stddef.h>
#include "game/functions.h"

#include "game/wave_player.h"

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
