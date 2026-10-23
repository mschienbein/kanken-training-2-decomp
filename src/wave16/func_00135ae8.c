#include "game/functions.h"
#include "game/scheduler.h"




/* C-only procedure callback: every caller discards its value (C11 6.9.1p12).
 * Do not consume the result or compile this definition as C++. */
int func_00135ae8(void)
{
    int slot = func_0019b564();

    if (func_0019b54c(slot)) {
        if (!func_0019f2e8())
            func_00190688(slot, func_0013db40);
    } else {
        func_00186944(slot, 1);
    }
}
