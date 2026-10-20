#include "game/bss_0049ecf0.h"


void func_00196114(unsigned mode)
{
    switch (mode) {
    case 0x400:
    case 0x401:
        bss_0049ecf0.resource_index = mode - 0x400;
        break;
    case 0x410:
        bss_0049ecf0.resource_index = 2;
        break;
    }
}
