#include "game/sound_format.h"

unsigned func_00140d78(const struct SoundRecord *record)
{
    unsigned value;
    if (!func_0018279c(&record->option_flags, &value, 2))
        return 0x40;
    return value & 0xff;
}
