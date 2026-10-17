#include "game/archive.h"

unsigned func_001f2334(const struct BankInstrumentInfo *info)
{
    unsigned value;
    if (func_0018279c(info->parameters, &value, 2))
        return value;
    return 0;
}
