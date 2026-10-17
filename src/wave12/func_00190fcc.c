#include "game/date_time.h"
#include "game/object_pool.h"
int func_00190fcc(const struct DateTime *time)
{
    int day;
    long long days = (time->milliseconds + 63082281600000LL) / 86400000;
    func_0018d074(0, 0, &day, days - 730119);
    return day;
}
