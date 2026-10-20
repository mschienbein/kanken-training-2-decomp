#include "game/selection.h"
struct SelectionAction {
    signed char kind;
    unsigned char reserved01[0x17];
    int enabled;
};


extern void func_001e6f40(struct SelectionAction *action);
extern void func_001e6df8(struct SelectionAction *action);
extern void func_001e612c(struct SelectionAction *action);
extern void func_001e6624(struct SelectionAction *action);
extern void func_001e6a14(struct SelectionAction *action);

void func_00172ea0(struct SelectionAction *action)
{
    if (action->enabled && action->kind) {
        switch (action->kind) {
        case 2:
            if (func_00174444())
                func_001e6df8(action);
            else
                func_001e612c(action);
            break;
        case 4:
            if (func_00174444())
                func_001e6f40(action);
            else
                func_001e6624(action);
            break;
        case 3:
            func_001e6a14(action);
            break;
        }
    }
}
