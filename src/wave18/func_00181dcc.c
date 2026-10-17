#include "game/service_state.h"

extern void func_001864cc(unsigned char suspended);
extern void func_001869f0(void);

void func_00181dcc(int reason, int enabled, int notify)
{
    if (reason) {
        if (enabled)
            data_0023fdd8.reasons |= 2;
        else
            data_0023fdd8.reasons &= ~2u;
    } else {
        if (enabled)
            data_0023fdd8.reasons |= 1;
        else
            data_0023fdd8.reasons &= ~1u;
    }
    if (data_0023fdd8.reasons) {
        if (notify)
            func_001864cc(1);
    } else {
        func_001869f0();
    }
    if (!enabled)
        func_001864cc(0);
}
