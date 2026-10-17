#include "game/sound_format.h"

unsigned func_00140c50(const struct SoundType1Details *info)
{
    unsigned value;
    if (func_0018279c(&info->option_flags, &value, 0))
        return value;
    return 0;
}
