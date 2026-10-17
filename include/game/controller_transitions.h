#ifndef GAME_CONTROLLER_TRANSITIONS_H
#define GAME_CONTROLLER_TRANSITIONS_H

struct TransitionScreenW19G;
struct RestartTransitionScreenW19G;
struct DetailedTransitionScreenW19G;
struct Screen;
struct MenuOptionGroup;
struct AnimationState;

#include "game/animation.h"
#include "game/deferred_callback.h"
#include "game/selection.h"
#include "game/w11_control.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */



struct TransitionScreenW19G : CallbackTarget {
    unsigned char reserved04[0x50];
    Screen *buttons;
    unsigned char reserved58[8];
    Screen *selector;
    unsigned char reserved64[0x2c];
    int phase;
    float progress;
    int previous_phase;
};
struct RestartTransitionScreenW19G : TransitionScreenW19G {
    int field9c;
    int fielda0;
    MenuOptionGroup *first;
    MenuOptionGroup *second;
};
struct DetailedTransitionScreenW19G : CallbackTarget {
    unsigned char reserved04[0x44];
    Screen *region;
    W6ListController *choices;
    unsigned reserved50;
    Screen *buttons;
    unsigned char reserved58[0x38];
    int phase;
    float progress;
    int previous_phase;
    int field9c;
};

extern "C" {



extern int func_0019efd0(void);
extern void func_00176d98(void);
extern void func_00176b24(Screen *set);
extern void func_0017697c(Screen *set);
extern int func_00170234(int channel);
extern void func_0017433c(int channel, int duration);
extern void func_00175848(void);


extern int data_0023f028;
extern void func_00181e44(void);
extern void func_001b6c90(MenuOptionGroup *resource);
extern void func_001b7e9c(MenuOptionGroup *resource);


extern Screen *func_001671d4(void);
extern AnimationState *func_00167284(void);
extern void func_0017673c(AnimationState *set);
extern void func_0017a258(AnimationState *set, float width, float height);
extern void func_00175d90(Screen *set);
extern void func_0016f9a8(Screen *set, int index, float x, float y);
extern void func_0017a034(Screen *set, int index, int value);
extern void func_001e9944(W6ListController *controller);
}
#endif

#endif
