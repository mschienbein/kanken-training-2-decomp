#ifndef GAME_TRANSITION_H
#define GAME_TRANSITION_H

#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/screen_operations.h"

struct TransitionCompletion {
    const void *vtable;
    unsigned char reserved04[0x30];
    int completion;
};
struct MenuTransitionState {
    struct TransitionCompletion completion_state;
    unsigned char reserved38[0x58];
    int stack_index;
    int modes[2];
};
#ifdef __cplusplus
extern "C" {
#endif
/* [INFERENCE] An erased completion setter is possible; historical identity unknown. */
struct TransitionCompletion *set_completion(struct TransitionCompletion *state, int completion);
void func_00172e00(struct MenuTransitionState *screen, int completion);
#ifdef __cplusplus
}
#endif

#endif
