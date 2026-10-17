#include "game/runtime.h"

void func_0012d2e8(void)
{
    int i;
    for (i = 0; i < 8; ++i) {
        if (bss_0049cb24.records[i].allocation) {
            func_0019dc20(bss_0049cb24.records[i].allocation);
            bss_0049cb24.records[i].allocation = 0;
            bss_0049cb24.records[i].count = 0;
        }
    }
}
