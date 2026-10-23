#include "game/sound_runtime.h"

struct SoundInstancePool;
struct WaveSoundControl {
    SoundInstance sound;
    void *source;
    SoundInstancePool *pool;
    unsigned char active;
    unsigned char enabled;
};

/* INTEGRATOR: move to include/game/sound_runtime.h */
extern "C" {
void func_0016a560(SoundInstance *sound);
void func_001d9a0c(WaveSoundControl *sound);
}

void func_001d9a0c(WaveSoundControl *sound)
{
    func_0016a560(&sound->sound);
    sound->source = 0;
    sound->active = 1;
    sound->enabled = 0;
}
