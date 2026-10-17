#include "game/date_time.h"
#include "game/object_pool.h"
int func_001efb00(const struct DateTime *time)
{
    int within_day = (time->milliseconds + 63082281600000LL) % 86400000;
    return within_day / 3600000 % 24;
}
