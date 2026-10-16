#include "game/screen.h"

void func_001b7530(struct Screen *screen, int enabled, int first, int second)
{
    func_0016e1d0(screen, 0x34, enabled);
    func_0016e1d0(screen, 0x35, enabled);
    if (enabled) {
        func_001b7c9c(screen, 0x34, first, screen->labels);
        func_001b7c9c(screen, 0x35, second, screen->labels);
    }
}
