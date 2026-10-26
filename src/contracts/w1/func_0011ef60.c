#include "game/functions.h"
#include "game/runtime.h"
#include "game/scheduler.h"
#include "game/startup.h"

/* [INFERENCE] Duration type identity: a time-span value built by a millisecond factory. */
struct TimeSpan {
    long long nanoseconds;
};

static struct TimeSpan time_span_from_milliseconds(int milliseconds)
{
    struct TimeSpan span;
    span.nanoseconds = (long long)milliseconds * 1000000;
    return span;
}

void func_0011ef60(void)
{
    while (func_0019f2e8())
        func_0011ef54(time_span_from_milliseconds(1).nanoseconds);
    while (func_0019efd0())
        func_0011ef54(time_span_from_milliseconds(1).nanoseconds);
    while (func_0012d37c())
        func_0011ef54(time_span_from_milliseconds(1).nanoseconds);
    func_001869f0();
    if (func_001869d0(18)) {
        func_00135b34();
        if (bss_00290c90.active == 3)
            func_00135450(0);
        if (bss_00290c90.active)
            bss_00290c90.active = 0;
        func_00195ad0(18);
    }
    func_0012d2bc();
    func_0012d2e8();
    func_00195b6c(0, 0, 0, 0, 0);
    func_001906a8(0);
    func_00195ad0(16);
}
