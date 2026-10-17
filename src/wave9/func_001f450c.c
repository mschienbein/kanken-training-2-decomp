#include "game/sound_format.h"

unsigned func_001f450c(const struct SoundRegion *region)
{
    unsigned value;
    if (func_0018279c(&region->option_flags, &value, 4))
        return (value >> 8) & 0xff;
    return 0;
}
