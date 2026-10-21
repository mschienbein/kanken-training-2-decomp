#ifndef GAME_ANIMATION_H
#define GAME_ANIMATION_H

/* Opaque intrusive-registry receiver; its +0x100 link is not in the 0xcc display pool owner. */
struct AnimatedObject;
struct AnimationIterator;
struct AnimationLinkPrefix;
struct DisplayObject;
struct NameTable;
struct PoolLink;
struct PoolList;
struct WidgetPosition;

#include <stdbool.h>
#include "game/audio.h"
#include "game/pool_list.h"

#include "game/screen.h"
#include "game/menu_controls.h"


typedef struct FrameTiming {
    unsigned int reserved;
    float rate;
    float milliseconds;
    float nominalFrames;
    unsigned char reserved10[0x0c];
    float interval;
    unsigned char reserved20[9];
    unsigned char flag29;
    unsigned char reserved2a;
    unsigned char flag2b;
    unsigned char reserved2c;
    unsigned char flag2d;
} FrameTiming;

#ifdef __cplusplus
extern "C" {
#endif
extern float func_0016702c(const struct InputRectangle *rectangle, float value);
extern float func_00167000(const struct InputRectangle *rectangle, float value);
extern void func_001670e4(struct InputRectangle *destination, float x, float y, float z, float w);
extern FrameTiming bss_00495b30;
extern float func_0017adb4(void);
extern float func_00191134(void);
extern int func_00174324(const struct MenuControlSet *owner, int index);
extern int func_001770b4(const struct MenuControlSet *owner, int index);
extern int func_0017710c(const struct MenuControlSet *owner, int index);
extern void func_001770cc(struct MenuControlSet *owner, int index, int enabled);
extern void func_00178fb0(const struct MenuControlSet *owner, int index,
                          float *width, float *height);
extern void func_00179030(const struct MenuControlSet *owner, int index,
                          int child_index, float *width, float *height);
#ifdef __cplusplus
}
#endif


struct AnimationLinkPrefix {
    unsigned char reserved00[0x100];
    struct PoolLink update_link;
};

#ifdef __cplusplus
struct AnimationIterator {
    PoolLink *node;
    AnimationIterator(PoolLink *value) : node(value) {}
    bool operator!=(const AnimationIterator &other) const
    {
        return node != other.node;
    }
    AnimatedObject &operator*() const
    {
        return *reinterpret_cast<AnimatedObject *>(reinterpret_cast<char *>(node) -
                    offsetof(AnimationLinkPrefix, update_link));
    }
    AnimationIterator operator++(int)
    {
        AnimationIterator old = *this;
        node = node->next;
        return old;
    }
};
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void func_00183f24(struct AnimatedObject *object, int frames);
extern void func_00130f54(struct PoolList *registry, int frames);
#ifdef __cplusplus
}
#endif

#endif
