#include "game/notification.h"

int func_001682b4(void)
{
    int pending = bss_00290cac.pending;
    if (pending == 6)
        return 1;
    if (pending == 7)
        return 2;
    if (pending != 8)
        return 0;
    return 3;
}
