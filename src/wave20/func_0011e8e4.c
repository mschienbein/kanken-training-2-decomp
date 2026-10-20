#include "game/bss_0049ecf0.h"

void func_0011e8e4(void)
{
    if (bss_0049ecf0.begin_callback != 0)
        bss_0049ecf0.begin_callback(0x400);
    bss_0049ecf0.begin_counter = (bss_0049ecf0.begin_counter + 1) % 0x7fffffff;
}
