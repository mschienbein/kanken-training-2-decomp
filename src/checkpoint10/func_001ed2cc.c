#include "game/controller_transitions.h"
#include "game/functions.h"
#include "game/screen.h"
#include "game/selection.h"

struct W24eCategoryState {
    unsigned char reserved00[0x48];
    struct Screen *numbers;
    struct W6ListController *list;
    unsigned char reserved50[8];
    struct W6Selector *selector;
    unsigned char reserved5c[0x54];
    int selection;
    int secondary;
};

extern void func_001741f8(struct Screen *screen, int index, float x, float y);


void func_001ed2cc(struct W24eCategoryState *state)
{
    func_00166f1c(state->numbers, 5, 0);
    func_00166f1c(state->numbers, 6, 0);
    switch (state->secondary) {
    case 1:
    case 3:
        func_001741f8(state->numbers, 5, -53.0f, 96.0f);
        func_001741f8(state->numbers, 6, 37.0f, -97.0f);
        func_00174210(state->selector, 116);
        break;
    case 11:
        func_001741f8(state->numbers, 5, -57.0f, 96.0f);
        func_001741f8(state->numbers, 6, 32.0f, -97.0f);
        func_00174210(state->selector, 117);
        break;
    default:
        func_001741f8(state->numbers, 5, -63.0f, 96.0f);
        func_001741f8(state->numbers, 6, 28.0f, -97.0f);
        func_00174210(state->selector, 115);
        break;
    }
    func_0017a034(state->numbers, 5, state->secondary);
    func_0017a034(state->numbers, 6, state->secondary);
    if (state->secondary >= 11)
        func_001780c4(state->list, 3, 1, 0);
    else
        func_001780c4(state->list, 3, 1, 1);
    if (state->secondary <= 0)
        func_001780c4(state->list, 3, 2, 0);
    else
        func_001780c4(state->list, 3, 2, 1);
}
