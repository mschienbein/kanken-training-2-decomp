#include "game/screen.h"

void func_001b7598(struct Screen *screen)
{
    int index;
    for (index = 0; index < screen->count; ++index) {
        func_0016e1d0(screen, index + 1, 0);
        func_0016e1d0(screen, index + 0x0d, 0);
        func_0016e1d0(screen, index + 0x19, 0);
        func_0016e1d0(screen, index + 0x25, 0);
    }
}
