#include "game/date_time.h"
#include "game/object_pool.h"
extern "C" void func_0015dc70(int *year, int *month, int *day, int *weekday,
                   int *hour, int *minute, int *second, int *millisecond)
{
    struct DateTime current;
    current = func_001910d4();
    if (year)
        *year = func_0019107c(&current);
    if (month)
        *month = func_00191024(&current);
    if (day)
        *day = func_00190fcc(&current);
    if (weekday)
        *weekday = func_001efb68(&current);
    if (hour)
        *hour = func_001efb00(&current);
    if (minute)
        *minute = func_001efbf8(&current);
    if (second)
        *second = func_001efc64(&current);
    if (millisecond)
        *millisecond = func_001efaac(&current);
}
