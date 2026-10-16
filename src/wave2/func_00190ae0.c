#include "game/graphics_state.h"

void *func_00190ae0(unsigned region)
{
    switch (region) {
    case 0x20000:
        func_00187818();
        return (void *)0x1f2fffffu;
    case 0x30000:
        func_00187818();
        return (void *)0x1f5fffffu;
    default:
        return 0;
    }
}
