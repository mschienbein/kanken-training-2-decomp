#include "game/startup.h"

void func_00105fa8(void)
{
    int i;
    bss_0049c878.a = 0;
    bss_0049c878.b = 0;
    bss_0049c878.c = 0;
    bss_0049c878.enabled = 0;
    bss_0049c878.active = 0;
    bss_0049c878.ready = 0;
    bss_0049c878.position = 0;
    bss_0049c878.step = 1;
    for (i = 0; i < 8; ++i) {
        bss_0049c878.items[i].value = 0;
        bss_0049c878.items[i].flags = 0;
        bss_0049c878.items[i].id = -1;
    }
    bss_0049c878.count = 0;
}
