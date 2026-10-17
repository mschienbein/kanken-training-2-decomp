#ifndef GAME_SOUND_H
#define GAME_SOUND_H

struct DeferredEntry;
struct DeferredQueue;
struct PoolIterator;
struct PoolLink;
struct PoolList;

#include "game/pool_list.h"
#include "game/runtime.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
struct PoolIterator {
    struct PoolLink *node;
};

struct DeferredQueue {
    unsigned char reserved00[0x18];
    struct PoolList active;
    unsigned char reserved24[0x14];
    unsigned int revision;
};

struct DeferredEntry {
    unsigned char reserved00[8];
    struct DeferredQueue *owner;
    unsigned char reserved0c[0x10];
    struct PoolLink link;
};

#ifdef __cplusplus
extern "C" {
#endif
extern void func_0014b5d8(struct DeferredQueue *queue, struct DeferredEntry *entry);
#ifdef __cplusplus
}
#endif

#endif
