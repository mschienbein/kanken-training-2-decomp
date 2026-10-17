#include "game/functions.h"
#include "game/runtime.h"

void func_001360a4(void)
{
    if (bss_002d4270.buffer) {
        if (bss_002d4270.owned)
            func_0019dc20(bss_002d4270.buffer);
        bss_002d4270.buffer = 0;
        bss_002d4270.count = 0;
        bss_002d4270.owned = 0;
    }
}
