#ifndef GAME_MOTION_SCREEN_H
#define GAME_MOTION_SCREEN_H
#include "game/runtime.h"

struct AnimatedWidget {
    unsigned flags;
    int enabled;
    unsigned char reserved08[0x48];
    float position[2];
    float displacement[2];
    float effective[2];
    float base[2];
    unsigned char reserved70[0x14];
    struct MotionState motion;
};

struct MotionScreen {
    unsigned char reserved00[0x20];
    struct AnimatedWidget *widgets[20];
};

#ifdef __cplusplus
extern "C" {
#endif
extern void func_0017457c(struct MotionScreen *screen);
#ifdef __cplusplus
}
#endif

#endif
