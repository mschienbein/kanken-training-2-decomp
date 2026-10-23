#include "game/selection.h"
struct W6HandlePanel10;
struct W6DisplayHandle;
struct W6SelectionReset {
    unsigned char reserved00[0x4c];
    struct W6ListController *list;
    unsigned char reserved50[0x0c];
    struct W6HandlePanel10 *panel;
    unsigned char reserved60[0x34];
    int mode;
    unsigned char reserved98[4];
    int previous_mode;
    unsigned char reserveda0[0x0c];
    struct W6DisplayHandle **handles;
};
extern void func_00171000(struct W6DisplayHandle **handles, int index, float x, float y);
extern void func_00173d8c(struct W6HandlePanel10 *panel, int resource);

void func_0016c55c(struct W6SelectionReset *state)
{
    func_00178e98(state->list, 0);
    if (state->previous_mode == 1 || state->previous_mode == 2)
        func_00171000(state->handles, -1, 0.0f, 0.0f);
    if (state->mode == 1) {
        func_00173d8c(state->panel, 27);
        func_00171000(state->handles, 8, 0.0f, 0.0f);
    } else if (state->mode == 2) {
        func_00173d8c(state->panel, 28);
        func_00171000(state->handles, 9, 0.0f, 0.0f);
    }
}
