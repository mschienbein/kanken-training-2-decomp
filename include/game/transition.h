#ifndef GAME_TRANSITION_H
#define GAME_TRANSITION_H

#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/screen_operations.h"

/* The transition receiver is passed unchanged to the member-callback dispatcher.
 * CallbackTarget supplies that evidenced virtual-slot interface, not a recovered
 * complete historical class hierarchy. */
#ifdef __cplusplus
struct MenuTransitionState : CallbackTarget {
#else
struct MenuTransitionState {
    const void *vtable;
#endif
    unsigned char reserved04[0x30];
    int completion;
    unsigned char reserved38[0x58];
    int stack_index;
    int modes[2];
};
#ifdef __cplusplus
extern "C" {
#endif
/* [INFERENCE] An erased completion setter is possible; historical identity unknown. */
struct MenuTransitionState *set_completion(struct MenuTransitionState *state, int completion);
void func_00172e00(struct MenuTransitionState *screen, int completion);
void func_00171208(struct MenuTransitionState *screen, int completion);
#ifdef __cplusplus
extern const MemberCallback data_00218648;
#endif
#ifdef __cplusplus
}
#endif

#endif
