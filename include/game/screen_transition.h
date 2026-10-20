#ifndef GAME_SCREEN_TRANSITION_H
#define GAME_SCREEN_TRANSITION_H

#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/category_progress.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */

extern "C" {
extern const SceneMemberCallback data_00218868;
extern void func_0017433c(int channel, int duration);
extern void func_00181b68(int index, int first, int second);
extern void func_00172d48(int index);
void func_00166700(CategoryScene *state, int mode);
}
#endif

#endif
