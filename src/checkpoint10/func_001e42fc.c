#include "game/animation.h"
#include "game/selection.h"

struct W8DisplaySet;
struct W8EffectState;
struct W8ScreenState {
    unsigned char reserved00[0x14];
    struct W8EffectState *effects;
    unsigned char reserved18[0x34];
    struct W6ListController *choices;
    struct W8DisplaySet *display;
    struct Screen *buttons;
    struct W6Selector *first_resource;
    struct W6HandlePanel10 *second_resource;
    struct Screen *selector;
    struct Screen *other;
    unsigned char reserved68[0x2c];
    int phase;
    unsigned char reserved98[4];
    int previous_phase;
    unsigned char reserveda0[0x10];
    int text_length;
    unsigned char reservedb4[0x0c];
    unsigned short text[25];
    unsigned char reservedf2[0x8e];
    struct DisplayObject *cursor;
};

struct W24ePhaseState {
    unsigned char reserved00[0x4c];
    struct AnimationOwner *animations;
    unsigned char reserved50[0x40];
    int current;
    int phases[3];
    unsigned char reserveda0[4];
    unsigned elapsed;
};

extern int func_00177124(unsigned mask);
extern void func_00181b68(unsigned mask, int active, int reset);
extern void func_001e403c(struct W8ScreenState *state);

void func_001e42fc(struct W24ePhaseState *state)
{
    switch (state->phases[state->current]) {
    case 2:
        func_001770cc(state->animations, 0, 0);
        func_001770cc(state->animations, 1, 0);
        func_001770cc(state->animations, 2, 0);
        func_001770cc(state->animations, 3, 0);
        func_001770cc(state->animations, 4, 0);
        func_001770cc(state->animations, 5, 0);
        func_00181b68(7, 1, 1);
        state->phases[state->current] = 3;
        state->elapsed = 0;
        break;
    case 3:
        if (!func_00177124(7)) {
            /* [INFERENCE] Both partial views describe this scene object. */
            func_001e403c((struct W8ScreenState *)state);
            func_00181b68(7, 0, 1);
            state->phases[state->current] = 4;
            state->elapsed = 0;
        }
        break;
    case 4: {
        int active = func_00177124(7);
        if (!active)
            state->current = active;
        break;
    }
    }
}
