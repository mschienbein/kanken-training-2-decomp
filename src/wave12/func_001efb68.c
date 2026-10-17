#include "game/date_time.h"
#include "game/object_pool.h"
unsigned char func_001efb68(const struct DateTime *time)
{
    long long days = (time->milliseconds + 63082281600000LL) / 86400000;
    int relative_day = days - 730119;
    return ((relative_day + 6) % 7 + 7) % 7;
}
