#include "game/startup.h"

void func_0010625c(void)
{
    if (bss_0049629c.callback) {
        bss_0049629c.callback();
        bss_0049629c.callback = 0;
    }
}
