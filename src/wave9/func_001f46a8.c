#include "game/sound_format.h"

unsigned func_001f46a8(const struct SoundRegion *region)
{
    unsigned value;
    if (!func_0018279c(&region->option_flags, &value, 1))
        return 127;
    return value & 0xff;
}
