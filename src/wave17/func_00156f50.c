#include "game/functions.h"
#include "game/hwr_runtime.h"

int func_00156f50(void)
{
    if (bss_004bfce0.state == 0)
        return -1;
    bss_004bfce0.state = 1;
    return 0;
}
