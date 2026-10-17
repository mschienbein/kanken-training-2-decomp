#include "game/sound_runtime.h"

void func_001d2c28(struct SoundInstance *sound, float volume)
{
    if (volume < 0.0f)
        volume = 0.0f;
    sound->volume = volume;
}
