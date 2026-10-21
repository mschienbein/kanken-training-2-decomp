#ifndef GAME_TRANSITION_CONTROLLER_H
#define GAME_TRANSITION_CONTROLLER_H
struct PairCollection;
#include "game/selection_collections.h"

struct MenuControlSet;
/* Existing accepted transition-controller view; field roles are descriptive. */
/* PairCollection is the native 001b6400 owner, including its transition tail. */
struct MenuExtentScene;
struct InitializationMenuScene;

#ifdef __cplusplus
extern "C" {
#endif
void func_00172c04(struct PairCollection *controller, int mode);
void func_00172c28(struct PairCollection *controller);
void func_00173f9c(struct PairCollection *controller, int paused);
void func_001b6788(struct PairCollection *controller, int mode);
void func_001a00f8(struct MenuExtentScene *screen);
void func_001a8048(struct InitializationMenuScene *screen);
/* [INFERENCE] Candidate-only active-byte accessor identities are not recovered. */
int transition_active(const struct PairCollectionData *controller);
signed char r4040_transition_active(const struct PairCollectionData *controller);
#ifdef __cplusplus
}
#endif

#endif
