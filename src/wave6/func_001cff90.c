#include "game/sound_runtime.h"

void func_001cff90(struct SoundHandle *handle, struct SoundInstance *sound)
{
    handle->sound = sound;
    if (func_001d2ce0(sound))
        func_00190634(handle->sound);
    handle->sound->handle = handle;
}
