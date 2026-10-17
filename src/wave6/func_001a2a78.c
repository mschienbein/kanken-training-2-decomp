#include "game/selection.h"


void func_001a2a78(struct W6SelectionScreen *screen, int from_list)
{
    int count = func_00171d24(func_001795bc());
    int kind;
    if (from_list) {
        int item;
        for (item = 0; item < count; ++item) {
            if (func_00174160(screen->list, func_00171c7c(screen), item) == 1)
                break;
        }
        if (item == count)
            return;
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
    case 0: func_00174210(screen->selector, 34); break;
    case 1: func_00174210(screen->selector, 35); break;
    case 2: func_00174210(screen->selector, 36); break;
    case 3: func_00174210(screen->selector, 37); break;
    case 4: func_00174210(screen->selector, 38); break;
    case 5: func_00174210(screen->selector, 39); break;
    case 6: func_00174210(screen->selector, 40); break;
    case 7: func_00174210(screen->selector, 41); break;
    case 8: func_00174210(screen->selector, 42); break;
    case 9: func_00174210(screen->selector, 43); break;
    case 10: func_00174210(screen->selector, 44); break;
    case 11: func_00174210(screen->selector, 45); break;
    case 12: func_00174210(screen->selector, 46); break;
    case 13: func_00174210(screen->selector, 47); break;
    case 14: func_00174210(screen->selector, 48); break;
    case 15: func_00174210(screen->selector, 49); break;
    case 16: func_00174210(screen->selector, 50); break;
    case 17: func_00174210(screen->selector, 51); break;
    case 18: func_00174210(screen->selector, 52); break;
    case 19: func_00174210(screen->selector, 53); break;
    case 20: func_00174210(screen->selector, 54); break;
    case 21: func_00174210(screen->selector, 55); break;
    case 22: func_00174210(screen->selector, 56); break;
    case 23: func_00174210(screen->selector, 57); break;
    case 24: func_00174210(screen->selector, 58); break;
    case 25: func_00174210(screen->selector, 59); break;
    case 26: func_00174210(screen->selector, 60); break;
    case 27: func_00174210(screen->selector, 61); break;
    case 28: func_00174210(screen->selector, 64); break;
    case 29: func_00174210(screen->selector, 65); break;
    case 30: func_00174210(screen->selector, 62); break;
    case 31: func_00174210(screen->selector, 63); break;
    default: func_00174210(screen->selector, 33); break;
    }
}
