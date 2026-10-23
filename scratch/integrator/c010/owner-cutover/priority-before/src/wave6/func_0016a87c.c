#include "game/sound_runtime.h"

void func_0016a87c(struct SoundInstance *sound, unsigned char priority, int offset)
{
    sound->priority_offset = offset;
    sound->priority = priority;
}
