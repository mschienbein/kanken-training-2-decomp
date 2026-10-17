#ifndef GAME_ANIMATION_H
#define GAME_ANIMATION_H

#include "game/screen.h"

struct AnimationChild {
    struct DisplayObject *object;
    const struct NameTable *table;
    short label_index;
    unsigned char reserved0a[2];
    unsigned short flags;
    unsigned char active;
    unsigned char pending;
    unsigned char reserved10;
    signed char text_index;
    short next_index;
    unsigned char reserved14[8];
    struct WidgetPosition position;
    float width;
    float height;
};

struct Animation {
    unsigned flags;
    unsigned char reserved04[4];
    struct AnimationChild **children;
    unsigned char reserved0c[0x34];
    float bounds[4];
    float width;
    float height;
    unsigned char reserved58[0x10];
    float elapsed;
    float previous;
    unsigned char reserved70[8];
    int frame_count;
    int frame;
};

struct AnimationOwner {
    unsigned reserved00[8];
#ifndef __cplusplus
    struct Animation *animations[];
#endif
};

typedef struct FrameTiming {
    unsigned int reserved;
    float rate;
    float milliseconds;
    float nominalFrames;
    unsigned char reserved10[0x0c];
    float interval;
} FrameTiming;

#ifdef __cplusplus
extern "C" {
#endif
extern float func_0016702c(const float *rectangle, float value);
extern float func_00167000(const float *rectangle, float value);
extern void func_001670e4(float *destination, float x, float y, float z, float w);
extern FrameTiming bss_00495b30;
extern float func_0017adb4(void);
extern float func_00191134(void);
extern int func_00174324(const struct AnimationOwner *owner, int index);
extern int func_001770b4(const struct AnimationOwner *owner, int index);
extern int func_0017710c(const struct AnimationOwner *owner, int index);
extern void func_001770cc(struct AnimationOwner *owner, int index, int enabled);
extern void func_00178fb0(const struct AnimationOwner *owner, int index,
                          float *width, float *height);
extern void func_00179030(const struct AnimationOwner *owner, int index,
                          int child_index, float *width, float *height);
#ifdef __cplusplus
}
#endif

#endif
