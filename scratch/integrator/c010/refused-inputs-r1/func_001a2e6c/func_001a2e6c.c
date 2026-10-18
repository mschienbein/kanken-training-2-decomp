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
extern void func_00166f1c(void *screen, int index, int enabled);
extern void func_001741f8(struct Screen *screen, int index, float x, float y);
extern void func_0017a034(struct Screen *screen, int index, int value);

void func_001a2e6c(struct W24eCategoryState *state, int find_selected)
{
    int selection;
    if (!find_selected) {
        selection = func_001795bc();
        state->selection = selection;
    } else {
        for (selection = 0; selection < 12; ++selection)
            if (func_00174160(state->list, 1, selection) == 1)
                break;
        if (selection == 12)
            return;
    }
    func_00166f1c(state->numbers, 3, 0);
    if (selection == -1) {
        func_00174210(state->selector, 29);
    } else if (selection == 1 || selection == 3) {
        func_00174210(state->selector, 31);
        func_001741f8(state->numbers, 3, -53.0f, 96.0f);
        func_0017a034(state->numbers, 3, selection);
    } else if (selection == 11) {
        func_00174210(state->selector, 32);
        func_001741f8(state->numbers, 3, -57.0f, 96.0f);
        func_0017a034(state->numbers, 3, selection);
    } else {
        func_00174210(state->selector, 30);
        func_001741f8(state->numbers, 3, -62.0f, 96.0f);
        func_0017a034(state->numbers, 3, selection);
    }
}
