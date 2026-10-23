#ifndef GAME_DRAWING_SCREEN_H
#define GAME_DRAWING_SCREEN_H

#include "game/animation.h"
#include "game/character_panel.h"
#include "game/deferred_callback.h"
#include "game/selection.h"
#include "game/screen_operations.h"

#ifdef __cplusplus
struct L035DrawingScreen {
    const void *vtable;
    unsigned char reserved04[0x10];
    W10DrawingInput *input;
    unsigned char reserved18[0x34];
    W6ListController *list;
    unsigned char reserved50[0x2c];
    CharacterPanel *panel;
    unsigned char reserved80[0x10];
    int mode;
    unsigned char reserved94[4];
    int states[3];
    int counter;
    unsigned char reserveda8[8];
    int selected;
    int count;
    unsigned char reservedb8[0x3f4];
    int phase;
    unsigned short recognized;

    void set_state(int state)
    {
        states[mode] = state;
        counter = 0;
    }
};
extern "C" {
void func_001bd68c(L035DrawingScreen *screen);
extern const MemberCallback data_00218498;
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
/* [INFERENCE] Candidate-only drawing-state accessor; historical identity unknown. */
int r41262_drawing_state(const struct W10DrawingInput *input);
#ifdef __cplusplus
}
#endif

#endif
