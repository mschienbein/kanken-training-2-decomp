#include "game/controller_transitions.h"
#include "game/functions.h"
#include "game/screen.h"
#include "game/selection.h"

struct W24eCategoryState {
    unsigned char reserved00[0x48];
    struct W11jVisualOwner *numbers;
    struct W6ListController *list;
    unsigned char reserved50[8];
    struct W6Selector *selector;
    unsigned char reserved5c[0x54];
    int selection;
    int secondary;
};



void func_001a79fc(struct W24eCategoryState *state, int find_selected)
{
    int selection;
    if (!find_selected) {
        selection = func_001795bc();
        state->selection = selection;
    } else {
        for (selection = 0; selection < 12; ++selection)
            if (func_00174160(state->list, 0, selection) == 1)
                break;
        if (selection == 12)
            return;
    }
    func_00166f1c(state->numbers, 0, 0);
    if (selection == -1) {
        func_00174210(state->selector, 68);
    } else if (selection == 1 || selection == 3) {
        func_00174210(state->selector, 71);
        func_001741f8(state->numbers, 0, -53.0f, 96.0f);
        func_0017a034(state->numbers, 0, selection);
    } else if (selection == 11) {
        func_00174210(state->selector, 72);
        func_001741f8(state->numbers, 0, -57.0f, 96.0f);
        func_0017a034(state->numbers, 0, selection);
    } else {
        func_00174210(state->selector, 70);
        func_001741f8(state->numbers, 0, -62.0f, 96.0f);
        func_0017a034(state->numbers, 0, selection);
    }
}
