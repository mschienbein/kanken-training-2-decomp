#include "game/sound_format.h"

unsigned func_00140bc4(const struct SoundType3Details *details)
{
    unsigned value;
    if (func_0018279c(&details->option_flags, &value, 0))
        return (value >> 8) & 0xff;
    return 0;
}
