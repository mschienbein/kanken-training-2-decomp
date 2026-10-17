#include "game/sound_format.h"

unsigned func_001f45e0(const struct SoundRegion *region)
{
    unsigned value;
    if (func_0018279c(&region->option_flags, &value, 4))
        return (value >> 16) & 0xff;
    return 0;
}
