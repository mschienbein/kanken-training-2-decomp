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

extern int func_00192250(unsigned char value, unsigned char *transitioned);

void func_0012f51c(void)
{
    int result;
    for (;;) {
        result = func_00192250(0, 0);
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
