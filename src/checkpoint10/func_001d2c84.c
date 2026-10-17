#include "game/sound_runtime.h"
#include "game/pool_list.h"


void func_001d2c84(struct SoundInstance *sound, int priority)
{
    sound->priority = priority;
    if (sound->player != 0)
        func_00143328(sound->player, sound);
    sound->vtable->priority_changed(sound);
}
