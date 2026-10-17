#include "game/sound_controller.h"
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
extern void func_00179d48(struct AnimationOwner *owner, int panel_index,
    int element_index, int enabled);
extern void func_001690a8(struct MenuTransitionState *state, int enabled);
extern void func_00169018(struct MenuTransitionState *state, int enabled);
extern void func_0016f174(struct MenuHint *hint, int enabled);

extern void func_001de2c0(struct MenuTransitionState *state);

void func_00168dd0(struct MenuTransitionState *state, int enabled)
{
    if (enabled) {
        func_00179d48(state->list.animation, state->index, 0, 1);
        func_00179d48(state->list.animation, state->index, 1, 0);
        func_001690a8(state, 0);
        func_00169018(state, 1);
        func_0016f174(state->hint, 0);
        func_00177598(state->motion, 0.0f, 0.0f);
    } else {
        func_00179d48(state->list.animation, state->index, 0, 0);
        func_00179d48(state->list.animation, state->index, 1, 1);
        func_001de2c0(state);
        func_00169018(state, 0);
    }
}
