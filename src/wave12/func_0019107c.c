#include "game/date_time.h"
#include "game/object_pool.h"
int func_0019107c(const struct DateTime *time)
{
    int year;
    long long days = (time->milliseconds + 63082281600000LL) / 86400000;
    func_0018d074(&year, 0, 0, days - 730119);
    return year;
}
