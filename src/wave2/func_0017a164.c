#include "game/screen.h"

void func_0017a164(struct Screen *screen, int state)
{
    switch (state) {
    case 0:
        func_0016e1d0(screen, 0x31, 0);
        func_0016e1d0(screen, 0x32, 0);
        break;
    case 1:
        func_0016e1d0(screen, 0x31, 1);
        func_0016e1d0(screen, 0x32, 1);
        break;
    case 2:
        func_0016e1d0(screen, 0x31, 1);
        func_0016e1d0(screen, 0x32, 0);
        break;
    case 3:
        func_0016e1d0(screen, 0x31, 0);
        func_0016e1d0(screen, 0x32, 1);
        break;
    }
}
