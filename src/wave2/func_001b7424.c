#include "game/screen.h"

static __forceinline void clearSelections(struct Screen *screen)
{
    int index;
    for (index = 0; index < screen->count; ++index) {
        func_0016e1d0(screen, index + 1, 0);
        func_0016e1d0(screen, index + 0x0d, 0);
        func_0016e1d0(screen, index + 0x19, 0);
        func_0016e1d0(screen, index + 0x25, 0);
    }
}

void func_001b7424(struct Screen *screen, int selected)
{
    int index;
    screen->selected = selected;
    clearSelections(screen);
    if (screen->mode == 3) {
        screen->selected = 0;
        func_0016e1d0(screen, 0x25, 1);
    } else {
        for (index = 0; index < screen->count; ++index) {
            if (screen->selected != index) {
                func_0016e1d0(screen, index + 1, 1);
            } else {
                func_0016e1d0(screen, index + 0x0d, 1);
                func_0016e1d0(screen, index + 0x19, 1);
                func_0016e1d0(screen, index + 0x25, 1);
            }
        }
    }
}
