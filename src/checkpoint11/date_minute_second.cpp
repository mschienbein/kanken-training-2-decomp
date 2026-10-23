#include "game/date_time.h"

/* [INFERENCE] Shared calendar epoch normalization for adjacent accessors. */
struct DayRemainder { long long milliseconds; };
static DayRemainder within_day(const DateTime *time)
{
    DayRemainder result;
    result.milliseconds = (time->milliseconds + 63082281600000LL) % 86400000;
    return result;
}
int func_001efbf8(const DateTime *time)
{
    return static_cast<int>(within_day(time).milliseconds) / 60000 % 60;
}
int func_001efc64(const DateTime *time)
{
    return static_cast<int>(within_day(time).milliseconds) / 1000 % 60;
}
