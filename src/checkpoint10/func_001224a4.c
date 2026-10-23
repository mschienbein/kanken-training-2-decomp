#include "game/runtime.h"

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

extern int func_00137f0c(unsigned flags);

void func_001224a4(void)
{
    int result;
    for (;;) {
        result = func_00137f0c(0x407);
        switch ((unsigned)result >> 31) {
        case 0:
            return;
        default:
            if ((unsigned)result == 0xc8a0cffcu)
                return;
        }
        if ((unsigned)result == 0xe0a0cc08u)
            func_0011ef54(time_span_from_milliseconds(10).nanoseconds);
    }
}
