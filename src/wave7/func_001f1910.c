#include "game/archive.h"

unsigned char func_001f1910(const unsigned *parameters)
{
    unsigned value;
    if (!func_0018279c(parameters, &value, 2))
        return 64;
    return (unsigned char)value;
}
