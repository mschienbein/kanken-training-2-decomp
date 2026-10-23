#include "game/archive.h"

unsigned char func_001f2374(const struct BankInstrumentInfo *info)
{
    unsigned value;
    if (!func_0018279c(info->parameters, &value, 0))
        return 1;
    return (unsigned char)value;
}
