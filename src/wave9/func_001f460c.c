#include "game/sound_format.h"

unsigned func_001f460c(const struct SoundRegion *region)
{
    unsigned value;
    if (!func_0018279c(&region->option_flags, &value, 2))
        return 64;
    return value & 0xff;
}
