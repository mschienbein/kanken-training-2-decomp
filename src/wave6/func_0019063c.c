#include "game/sound_runtime.h"

void func_0019063c(struct SoundHandle *handle)
{
    if (handle->sound != 0) {
        if (handle->sound->handle == handle)
            handle->sound->handle = 0;
        if (handle->sound->secondary_handle == handle)
            handle->sound->secondary_handle = 0;
        if (handle->sound != 0)
            handle->sound = 0;
    }
}
