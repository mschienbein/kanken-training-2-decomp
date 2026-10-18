#include "game/animation.h"
#include "game/selection.h"

struct MenuFeedback;
struct MenuHint;
struct MenuMotion;
struct MenuTransitionState {
    unsigned char reserved00[0x14];
    struct MenuFeedback *feedback;
    unsigned char reserved18[0x34];
    union {
        struct AnimationOwner *animation;
        struct W6ListController *selection;
    } list;
    unsigned char reserved50[0x1c];
    struct MenuHint *hint;
    unsigned char reserved70[8];
    struct MenuMotion *motion;
    unsigned char reserved7c[0x80];
    int index;
};
extern void func_00168dd0(struct MenuTransitionState *state, int enabled);
extern void func_0016dbc4(struct MenuFeedback *feedback);

int func_001689c8(struct MenuTransitionState *state)
{
    int result = func_0017710c(state->list.animation, state->index);
    if (result == 0) {
        func_00168dd0(state, 0);
        func_00178e98(state->list.selection, state->index);
    } else if (result == 1) {
        func_00168dd0(state, 1);
        func_00178e98(state->list.selection, state->index);
    }
    if (result != -1)
        func_0016dbc4(state->feedback);
    return result;
}
