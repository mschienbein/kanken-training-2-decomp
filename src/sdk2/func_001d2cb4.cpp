#include "game/sound_runtime.h"
int func_001d2cb4(const SoundPriorityHook *hook, unsigned int sound_id)
{
    return hook->priorityOffset(sound_id);
}
