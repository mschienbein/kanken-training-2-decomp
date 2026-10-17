#include "game/sound_format.h"

const struct SoundRegionFixed *func_001f45c8(const struct SoundRegion *region)
{
    if (region->option_flags != 0x21f)
        return 0;
    return (const struct SoundRegionFixed *)region->parameters;
}
