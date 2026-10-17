#include "game/date_time.h"
#include "game/object_pool.h"
int func_00191024(const struct DateTime *time)
{
    int month;
    long long days = (time->milliseconds + 63082281600000LL) / 86400000;
    func_0018d074(0, &month, 0, days - 730119);
    return month;
}
