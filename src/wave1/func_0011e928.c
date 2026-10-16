#include "game/bss_0049ecf0.h"

void func_0011e928(void)
{
    if (bss_0049ecf0.callback != 0)
        bss_0049ecf0.callback(0x401);
    bss_0049ecf0.counter = (bss_0049ecf0.counter + 1) % 0x7fffffff;
}
