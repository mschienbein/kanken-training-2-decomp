#include "game/sound_format.h"

unsigned func_00140cf0(const struct SoundRecord *record)
{
    unsigned value;
    if (func_0018279c(&record->option_flags, &value, 1))
        return (value >> 8) & 0xff;
    return 0;
}
