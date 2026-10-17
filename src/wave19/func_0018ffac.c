#include "game/random.h"

int func_0018ffac(int limit)
{
    unsigned short value;
    data_00242240.seed.unsigned_value = data_00242240.seed.unsigned_value * 549 - 1;
    value = data_00242240.seed.unsigned_value;
    if (limit == 0)
        return data_00242240.seed.signed_value;
    if (limit > 0)
        return value * limit / 65536;
    return data_00242240.seed.signed_value * limit / 32768;
}
