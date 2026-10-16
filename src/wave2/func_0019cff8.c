#include "game/graphics_state.h"

void *func_0019cff8(unsigned region)
{
    switch (region) {
    case 0x20000:
        return func_00187818() ? (void *)0x1f180000u : (void *)0x1f000000u;
    case 0x30000:
        return func_00187818() ? (void *)0x1f380000u : (void *)0x1f300000u;
    default:
        return 0;
    }
}
