#ifndef GAME_DRAWING_SCREEN_H
#define GAME_DRAWING_SCREEN_H

#include "game/animation.h"
#include "game/character_panel.h"
#include "game/deferred_callback.h"
#include "game/selection.h"
#include "game/screen_operations.h"

#ifdef __cplusplus
struct TrainingScreenB : MenuScene {
    virtual void build();
    virtual ~TrainingScreenB();
    virtual void activate();
    virtual void update();
    virtual void depart();
    virtual void prepare();
    /* [INFERENCE] func_001bd944 constructs this concrete class.
     * Slot 7 override was eliminated (null cell); its signature is
     * unrecoverable beyond the root placeholder. Never call or bind it. */
    virtual void reserved_slot7();
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
    unsigned char reserved4b2[0x1e];
    W11ControlScreen *small_controls;

    void set_state(int state)
    {
        states[mode] = state;
        counter = 0;
    }
};
extern "C" {
void func_001bd68c(TrainingScreenB *screen);
void func_001bbdd4(TrainingScreenB *screen);
void func_001bc2a0(TrainingScreenB *screen);
void func_001bbfc4(TrainingScreenB *screen);
extern const SceneMemberCallback data_00218498;
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
