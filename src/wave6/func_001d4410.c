#include "game/sound_runtime.h"

int func_001d4410(const struct SoundSelectionList *selection, int requested_priority)
{
    struct SoundInstance *sound;
    int priority;
    if (selection->limit == 0)
        goto reject;
    if (selection->limit > (int)selection->sounds.count)
        goto accept;
    sound = func_001d4464(selection);
    if (sound == 0)
        return 0;
    priority = __usat(sound->priority + sound->priority_offset, 7);
    if (priority <= requested_priority)
        goto accept;
reject:
    return 0;
accept:
    return 1;
}
