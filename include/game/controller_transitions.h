#ifndef GAME_CONTROLLER_TRANSITIONS_H
#define GAME_CONTROLLER_TRANSITIONS_H

struct Screen;
struct MenuOptionGroup;
struct AnimationState;

#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/selection.h"
#include "game/w11_control.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */



struct DepartureScene : MenuScene {
    virtual void build();
    virtual ~DepartureScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    /* [INFERENCE] Concrete constructor func_001e744c; slot 6
     * override eliminated (null cell). Declaration only, never called. */
    virtual void prepare();
    /* [INFERENCE] func_001e744c constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    int phase;
    float progress;
    int previous_phase;
};
struct ReturnScene : MenuScene {
    virtual void build();
    virtual ~ReturnScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    /* [INFERENCE] Concrete constructor func_001e7fb8; slot 6
     * override eliminated (null cell). Declaration only, never called. */
    virtual void prepare();
    /* [INFERENCE] func_001e7fb8 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    int phase;
    float progress;
    int previous_phase;
};
struct CompletionScene : MenuScene {
    virtual void build();
    virtual ~CompletionScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    /* [INFERENCE] Concrete constructor func_001ec128; slot 6
     * override eliminated (null cell). Declaration only, never called. */
    virtual void prepare();
    /* [INFERENCE] func_001ec128 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    int phase;
    float progress;
    int previous_phase;
};
struct DetailedTransitionScene : MenuScene {
    virtual void build();
    virtual ~DetailedTransitionScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    virtual void prepare();
    /* [INFERENCE] func_001e7c4c constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    int phase;
    float progress;
    int previous_phase;
    int field9c, fielda0;
    MenuOptionGroup *first, *second;
};
struct IntroScene : MenuScene {
    virtual void build();
    virtual ~IntroScene();
    virtual void activate();
    virtual void update();
    virtual void depart();
    /* [INFERENCE] Concrete constructor func_001e3ea4; slot 6
     * override eliminated (null cell). Declaration only, never called. */
    virtual void prepare();
    /* [INFERENCE] func_001e3ea4 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
    int phase;
    float remaining;
    int sound, page, question;
    MenuOptionGroup *first_options, *second_options;
};

extern "C" {



extern int func_0019efd0(void);
extern void func_00176d98(void);
extern void func_00176b24(SelectionList24 *set);
extern void func_0017697c(SelectionList29 *set);
extern int func_00170234(int channel);
extern void func_0017433c(int channel, int duration);
extern void func_00175848(void);


extern int data_0023f028;
extern void func_001b6c90(MenuOptionGroup *resource);
extern void func_001b7e9c(MenuOptionGroup *resource);


extern SelectionList29 *func_001671d4(void);
extern void func_0017673c(AnimationState *set);
void func_001b3c0c(AnimationState *slider, int delta);
extern void func_0017a258(AnimationState *set, float width, float height);
extern void func_001e9944(MenuControlSet *controller);
}
#endif

#endif
