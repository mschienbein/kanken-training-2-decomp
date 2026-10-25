#include "game/calendar_time.h"

TimestampBuffers::TimestampBuffers()
{
    func_0019df08(first, sizeof(first));
    func_0019df08(second, sizeof(second));
    func_0019df08(third, sizeof(third));
    timestamp = CalendarTime(1970, 1, 1, 0, 0, 0, 0);
    active = 0;
}
