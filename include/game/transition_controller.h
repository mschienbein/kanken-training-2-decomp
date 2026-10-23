#ifndef GAME_TRANSITION_CONTROLLER_H
#define GAME_TRANSITION_CONTROLLER_H

struct AnimationOwner;
/* Existing accepted transition-controller view; field roles are descriptive. */
struct TransitionController {
    unsigned char reserved00[0x34];
    unsigned char active;
    unsigned char paused;
    unsigned char reserved36[2];
    float elapsed;
    unsigned char reserved3c[4];
    int mode;
};
struct W11jTransitionScreen {
    unsigned char reserved00[0x4c];
    struct AnimationOwner *animations;
    unsigned char reserved50[0x3c];
    struct TransitionController *group;
    int depth;
#ifndef __cplusplus
    int stages[];
#endif
};

#ifdef __cplusplus
extern "C" {
#endif
void func_00172c04(struct TransitionController *controller, int mode);
void func_00172c28(struct TransitionController *controller);
void func_00173f9c(struct TransitionController *controller, int paused);
void func_001b6788(struct TransitionController *controller, int mode);
void func_001a00f8(struct W11jTransitionScreen *screen);
void func_001a8048(struct W11jTransitionScreen *screen);
/* [INFERENCE] Candidate-only active-byte accessor identities are not recovered. */
int transition_active(const struct TransitionController *controller);
signed char r4040_transition_active(const struct TransitionController *controller);
#ifdef __cplusplus
}
#endif

#endif
