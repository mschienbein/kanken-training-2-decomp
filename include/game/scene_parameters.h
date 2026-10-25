#ifndef GAME_SCENE_PARAMETERS_H
#define GAME_SCENE_PARAMETERS_H
#include "game/playback_screen.h"
#include "game/controller_transitions.h"

/* Exact existing parameter-buffer provider prefix; no alternate opaque
 * receiver is retained for the scene that calls it. */
struct ParameterBufferState {
    unsigned char reserved00[0x0c];
    float read_index;
    float write_index;
    unsigned position;
    unsigned char reserved18[0x10];
    unsigned count;
    void *buffer;
};

#ifdef __cplusplus
/* [INFERENCE] Descriptive value-construction roles; these classes add no
 * storage to the canonical ParameterValue prefix. */
struct ParameterSelection : ParameterValue {
    explicit ParameterSelection(int initial) { value = initial; }
    ParameterSelection();
};
struct ParameterSequence : ParameterValue {
    explicit ParameterSequence(int initial) { value = initial; }
    ParameterSequence();
};
struct Tu1140ParameterScene : CallbackTarget {
    unsigned char reserved04[0x50];
    Screen *selection;
    unsigned char reserved58[8];
    Screen *sequence;
    unsigned char reserved64[0x14];
    ParameterBufferState *buffer;
};

extern "C" {
extern const MemberCallback data_002184b8;
extern const MemberCallback data_00218888;
void func_001e395c(RestartTransitionScreenW19G *state);
void func_001e722c(Tu1140ParameterScene *state);
}
#endif
#ifdef __cplusplus
extern "C" {
#endif
void func_001757dc(struct ParameterBufferState *state);
#ifdef __cplusplus
}
#endif
#endif
