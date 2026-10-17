#include "game/sound_format.h"
_Bool func_001f459c(const struct SoundRegion *region)
{
    unsigned value;
    if (!func_0018279c(&region->option_flags, &value, 4))
        return 0;
    return (value & 0xff) != 0;
}
