#include "game/selection.h"


struct W16RestoreState {
    unsigned char reserved00[0x48];
    struct VisualLabelSet72 *widgets;
    unsigned char reserved4c[0xc];
    struct W6Selector *selector;
    unsigned char reserved5c[0x60];
    int resource;
    int first;
    int first_value;
    int second;
    int second_value;
    float first_position[2];
    float second_position[2];
};


void func_001732b4(struct W16RestoreState *state)
{
    int resource = state->resource;
    if (resource == 19) {
        func_00174210(state->selector, 19);
    } else if (resource == 24) {
        func_00174210(state->selector, 24);
    } else {
        func_00174210(state->selector, resource);
        if (state->first != -1) {
            func_001741f8(state->widgets, state->first,
                           state->first_position[0], state->first_position[1]);
            func_0017a034(state->widgets, state->first, state->first_value);
        }
        if (state->second != -1) {
            func_001741f8(state->widgets, state->second,
                           state->second_position[0], state->second_position[1]);
            func_0017a034(state->widgets, state->second, state->second_value);
        }
    }
}
