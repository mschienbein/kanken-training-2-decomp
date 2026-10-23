#include "game/archive.h"

bool func_001f23b8(const struct BankInstrumentInfo *info)
{
    unsigned value;
    if (!func_0018279c(info->parameters, &value, 0))
        return false;
    return (unsigned char)(value >> 8) == 1;
}
