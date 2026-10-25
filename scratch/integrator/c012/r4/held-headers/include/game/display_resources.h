#ifndef GAME_DISPLAY_RESOURCES_H
#define GAME_DISPLAY_RESOURCES_H
#include "game/screen.h"
#ifdef __cplusplus
#include <preinclude.h>
#include <nw/lyt/lyt_Layout.h>
#include <nw/lyt/lyt_ResourceAccessor.h>
/* Names-admission's exact animation loader supplies these slot offsets and
 * native middleware interfaces. [INFERENCE] Game-owned descriptive identities. */
struct W16AnimationSlot {
    unsigned char reserved00[8];
    float frame;
    unsigned char reserved0c[8];
    int mode;
    float progress;
    nw::lyt::Layout *player;
    nw::lyt::AnimTransform *animation;
    unsigned char reserved24[0x10];
    nw::lyt::ResourceAccessor *accessor;
};
struct W16AnimatedDisplay {
    DisplayObject base;
    W16AnimationSlot *slots[2];
    __forceinline W16AnimationSlot *animationSlot(int index) { return slots[index]; }
};

#endif
#ifdef __cplusplus
extern "C" {
#endif
extern char bss_0049d958[];
#ifdef __cplusplus
}
#endif
#endif
