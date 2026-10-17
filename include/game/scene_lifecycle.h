#ifndef GAME_SCENE_LIFECYCLE_H
#define GAME_SCENE_LIFECYCLE_H

#include <stdbool.h>
#include "game/functions.h"

struct ManagedHandle;
struct ManagedResource {
    struct ManagedHandle *handles[2];
    int count;
    unsigned char reserved0c[8];
    unsigned char registered;
    unsigned char reserved15[2];
    signed char enabled;
    unsigned char reserved18[8];
    unsigned short flags;
    unsigned char reserved22[0x36];
    struct PoolLink link;
};

struct SceneChild {
    unsigned char reserved00[0x130];
    struct SceneChild *children;
    unsigned char reserved134[4];
    unsigned char enabled;
    unsigned char active;
    unsigned char state;
    unsigned char reserved13b[0x65];
    void (*callback)(struct SceneChild *child, unsigned event, void *context);
    void *context;
    struct ManagedResource *element;
    struct SceneChild *next;
    struct PoolLink link;
};

struct SceneEngine {
    unsigned char reserved00[0x51];
    unsigned char inhibited;
    unsigned char reserved52[0x32];
    struct PoolList active;
};

struct ObjectPool;
#ifdef __cplusplus
extern "C" {
#endif
extern struct ObjectPool *func_00185cc0(void);
extern void func_00182994(struct ObjectPool *pool, struct SceneChild *child);
extern void func_001829d0(struct ManagedHandle *handle);
extern void func_00183550(struct ManagedResource *resource);
extern void func_0016a27c(struct ManagedResource *resource, int enabled);
extern void func_001d4cf4(struct SceneChild *parent, bool enabled);
#ifdef __cplusplus
}
#endif

#endif
