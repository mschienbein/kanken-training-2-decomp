#ifndef GAME_SCREEN_TRANSITION_H
#define GAME_SCREEN_TRANSITION_H

#include "game/animation.h"
#include "game/deferred_callback.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
struct ScreenTransition;

struct ScreenTransition {
    unsigned char reserved00[0x34];
    int mode;
    unsigned char reserved38[0x58];
    int current_slot;
    int current[2];
    int previous[2];
    int pending;
    unsigned char reserveda8[8];
    int notification_mode;
};

extern "C" {
extern const MemberCallback data_00218868;
extern void func_00166ae8(ScreenTransition *state, int value, int slot);
extern void func_0017433c(int channel, int duration);
extern void func_00181b68(int index, int first, int second);
extern void func_00172d48(int index);
void func_00166700(ScreenTransition *state, int mode);
}
#endif

/* [INFERENCE] Fixed-stride animation prefix, distinct from ScreenTransition. */
struct ScreenAnimation {
    int mode;
    unsigned char reserved04[0x1c];
    const struct NameTable *table;
    unsigned char reserved24[0x10];
    struct BoundDisplayObject *objects[5];
    unsigned char reserved48[0xc];
};
struct TransitionScene {
    unsigned char reserved00[0x18];
#ifndef __cplusplus
    struct ScreenAnimation animations[];
#endif
};
/* [INFERENCE] Settings receiver from its natural caller, not ScreenTransition. */
struct SettingsScreen {
    unsigned char reserved00[0x34];
    int next_state;
    unsigned char reserved38[0x14];
    struct W6ListController *controls;
    unsigned char reserved50[0x40];
    int state;
    unsigned char reserved94[8];
    int pending_state;
    int previous_state;
    int transition;
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_0016f39c(struct TransitionScene *scene, int index);
extern void func_0016f3a0(struct TransitionScene *scene, int index);
extern void func_0016f534(struct BoundDisplayObject *object, int index, float duration);
extern void func_0016fc1c(struct SettingsScreen *screen, int state, int flags, int transition);
extern void func_0016fa58(struct SettingsScreen *screen, int enabled);
extern void func_001b06a4(struct SettingsScreen *screen);
#ifdef __cplusplus
}
#endif

#endif
