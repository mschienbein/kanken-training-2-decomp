#include "game/w11_control.h"
void func_0016d554(struct W11ControlScreen *screen, int state)
{
    switch (state) {
    case 0:
        func_0016ff68(screen, 0, 0);
        func_0016ff68(screen, 1, 0);
        break;
    case 1:
        func_0016ff68(screen, 0, 1);
        func_0016ff68(screen, 1, 1);
        break;
    case 2:
        func_0016ff68(screen, 0, 1);
        func_0016ff68(screen, 1, 0);
        break;
    case 3:
        func_0016ff68(screen, 0, 0);
        func_0016ff68(screen, 1, 1);
        break;
    }
}
