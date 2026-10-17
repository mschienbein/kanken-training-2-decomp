#include "game/screen.h"

struct W11ControlScreen {
    unsigned char reserved00[0x0c];
    struct WidgetSlot *slots[10];
    int layout;
    int active;
    int selection;
};

extern void func_0016ff68(struct W11ControlScreen *screen, int index, int enabled);

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
