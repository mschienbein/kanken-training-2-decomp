#ifndef GAME_CHOICE_SCREEN_H
#define GAME_CHOICE_SCREEN_H
#include "game/screen.h"
#include "game/selection.h"

/* Accepted choice-scene view. The +0x4c list owner is not equated with the
 * AnimationOwner view in the deferred phase dispatcher without owner evidence. */
struct W8ScreenState {
    unsigned char reserved00[0x14];
    struct W10DrawingInput *effects;
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
#ifdef __cplusplus
extern "C" {
#endif
extern void func_001e403c(struct W8ScreenState *state);
#ifdef __cplusplus
}
#endif
#endif
