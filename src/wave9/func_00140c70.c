#include "game/sound_format.h"

unsigned func_00140c70(const struct SoundType1Details *details)
{
    unsigned value;
    if (!func_0018279c(&details->option_flags, &value, 1))
        return 0x40;
    return value & 0xff;
}
