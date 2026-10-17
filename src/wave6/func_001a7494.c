#include "game/selection.h"


static __inline int item_value(struct W6SelectionScreen *screen, int item)
{
    int list = func_001718ec(screen);
    return func_00174160(screen->list, list, item);
}

void func_001a7494(struct W6SelectionScreen *screen, int from_list)
{
    int count = func_00171d24(func_001795bc());
    int kind;
    if (from_list) {
        int item;
        ++count;
        for (item = 0; item < count; ++item) {
            if (item_value(screen, item) == 1)
                break;
        }
        if (item == count)
            return;
        if (func_00171d24(func_001795bc()) <= item) {
            func_00174210(screen->selector, 102);
            return;
        }
        kind = func_00171c58(func_001795bc(), item);
    } else {
        goto load_current;
    }
dispatch:
    switch (kind) {
load_current:
        kind = func_00178144();
        screen->selection = kind;
        goto dispatch;
    case 0: func_00174210(screen->selector, 73); break;
    case 1: func_00174210(screen->selector, 74); break;
    case 2: func_00174210(screen->selector, 75); break;
    case 3: func_00174210(screen->selector, 76); break;
    case 4: func_00174210(screen->selector, 77); break;
    case 5: func_00174210(screen->selector, 78); break;
    case 6: func_00174210(screen->selector, 79); break;
    case 7: func_00174210(screen->selector, 80); break;
    case 8: func_00174210(screen->selector, 81); break;
    case 9: func_00174210(screen->selector, 82); break;
    case 10: func_00174210(screen->selector, 83); break;
    case 11: func_00174210(screen->selector, 84); break;
    case 12: func_00174210(screen->selector, 85); break;
    case 13: func_00174210(screen->selector, 86); break;
    case 14: func_00174210(screen->selector, 87); break;
    case 15: func_00174210(screen->selector, 88); break;
    case 16: func_00174210(screen->selector, 89); break;
    case 17: func_00174210(screen->selector, 90); break;
    case 18: func_00174210(screen->selector, 91); break;
    case 19: func_00174210(screen->selector, 92); break;
    case 20: func_00174210(screen->selector, 93); break;
    case 21: func_00174210(screen->selector, 94); break;
    case 22: func_00174210(screen->selector, 95); break;
    case 23: func_00174210(screen->selector, 96); break;
    case 24: func_00174210(screen->selector, 97); break;
    case 25: func_00174210(screen->selector, 98); break;
    case 26: func_00174210(screen->selector, 99); break;
    case 27: func_00174210(screen->selector, 100); break;
    case 28: func_00174210(screen->selector, 103); break;
    case 29: func_00174210(screen->selector, 104); break;
    case 30: func_00174210(screen->selector, 101); break;
    case 31: func_00174210(screen->selector, 102); break;
    default: func_00174210(screen->selector, 69); break;
    }
}
