#ifndef GAME_MENU_CONTROLLER_H
#define GAME_MENU_CONTROLLER_H

#include "game/animation.h"
#include "game/selection.h"
#include "game/transition_controller.h"

struct HorizontalLabelLayout;
struct Tu441SecondaryController;
struct MenuControllerState;
#ifdef __cplusplus
/* [INFERENCE] Native virtual-operation names and historical class identity. */
struct MenuControllerState {
    virtual void reserved00();
    virtual void reserved04();
    virtual void reserved08();
    virtual void reserved0c();
    virtual void reserved10();
    virtual void reserved14();
    virtual void before_update();
    virtual void reserved1c();
    virtual void after_update();
    unsigned char reserved04_data[0x44];
    struct HorizontalLabelLayout *labels;
    struct AnimationOwner *animations;
    unsigned char reserved50[0x3c];
    struct TransitionController *group;
    int depth;
    /* [INFERENCE] Capacity follows the next observed member at +0xe0. */
    int stages[19];
    void *member;
    struct Tu441SecondaryController *secondary_member;
};
extern "C" {
#endif
/* [INFERENCE] Erased secondary update and state-accessor identities remain unknown. */
void tu441_update_secondary(struct Tu441SecondaryController *controller);
signed char tu441_transition_active(const struct TransitionController *controller);
int func_00171e30(void);
void func_001c1080(void *object);
void func_0016337c(struct MenuControllerState *state, int current);
int func_00171d88(const struct AnimationOwner *owner, int index, int child_index);
void func_00171d40(struct AnimationOwner *owner, int index, int child_index, int text);
void func_00171db8(struct AnimationOwner *owner, int index, int child_index, float x, float y);
void func_001ea6d8(struct HorizontalLabelLayout *layout, int index, float x, float y);
void func_001a2814(struct MenuControllerState *state);
void func_001a3ee0(struct MenuControllerState *screen);
#ifdef __cplusplus
}
#endif

#endif
