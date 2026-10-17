#include "game/bss_0049ecf0.h"
/* INTEGRATOR: differs from include/game/bss_0049ecf0.h: its reserveda4
 * span contains the writable selected-resource index at aggregate offset 0x124. */
struct CommandSystem {
    unsigned char reserved00[0x124];
    int resource_index;
};


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
