#include "game/functions.h"
#include "game/service_state.h"




void func_0019eb60(int reason, int enabled)
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
    if (data_0023fdd8.reasons)
        func_001864cc(1);
    else
        func_001869f0();
    if (!enabled)
        func_001864cc(0);
}
