#include "game/sound_format.h"

unsigned func_00140b9c(const struct SoundType3Details *details)
{
    unsigned value;
    if (!func_0018279c(&details->option_flags, &value, 0))
        return 0x40;
    return value & 0xff;
}
