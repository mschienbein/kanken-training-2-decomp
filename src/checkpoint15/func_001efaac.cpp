#include "game/date_time.h"

/* [INFERENCE] Typed duration/component ownership boundary. */
struct DayTime {
    int milliseconds;
    explicit DayTime(const DateTime &time)
        : milliseconds((time.milliseconds + 63082281600000LL) % 86400000) {}
    int component() const { return milliseconds % 1000; }
};
int func_001efaac(const DateTime *time)
{
    return DayTime(*time).component();
}
