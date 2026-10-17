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

#endif
