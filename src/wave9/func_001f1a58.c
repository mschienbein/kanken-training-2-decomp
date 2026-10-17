#include "game/sound_format.h"

unsigned char func_001f1a58(const struct SoundOptionalParameters *note)
{
    unsigned value;
    if (func_0018279c(&note->flags, &value, 2))
        return (unsigned char)(value >> 8);
    return 0;
}
