#include "game/sound_format.h"

signed char func_001f1aa8(const struct SoundOptionalParameters *note)
{
    unsigned value;
    if (func_0018279c(&note->flags, &value, 0))
        return (signed char)(value >> 8);
    return 0;
}
