#include "game/sound_format.h"

unsigned func_001f4574(const struct SoundRegion *region)
{
    unsigned value;
    if (!func_0018279c(&region->option_flags, &value, 0))
        return 60;
    return value & 0xff;
}
