#include "game/archive.h"

unsigned char func_00140ccc(const struct SoundRecord *record)
{
    unsigned value;
    if (func_0018279c(&record->option_flags, &value, 1))
        return (unsigned char)value;
    return 0;
}
