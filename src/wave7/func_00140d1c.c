#include "game/archive.h"

int func_00140d1c(const struct SoundRecord *record)
{
    unsigned value;
    if (func_0018279c(&record->option_flags, &value, 17))
        return value & 1;
    return 0;
}
