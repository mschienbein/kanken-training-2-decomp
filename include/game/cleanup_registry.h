#ifndef GAME_CLEANUP_REGISTRY_H
#define GAME_CLEANUP_REGISTRY_H

struct CleanupContext;
struct CleanupGroup;
struct CleanupNode;
struct FrameHeap;
struct PoolLink;
struct PoolList;

#include "game/pool_list.h"
#include "game/runtime.h"
#include "game/stream_manager.h"

/* [INFERENCE] Descriptive names; widths and offsets follow matching clients. */
typedef void (*CleanupCallback)(void *memory, unsigned size, void *cookie);

#ifdef __cplusplus
struct CleanupNode {
    PoolLink link;
    void *memory;
    unsigned size;
    CleanupCallback callback;
    void *cookie;
    unsigned reserved18[2];
    CleanupNode(void *allocation, unsigned length, CleanupCallback cleanup, void *argument)
        : link(), memory(allocation), size(length), callback(cleanup), cookie(argument) {}
};
#endif

struct CleanupGroup {
    struct PoolLink link;
    struct PoolList callbacks;
    unsigned char active;
};

struct CleanupContext {
    struct FrameHeap *heap;
    struct PoolList groups;
};

/* [INFERENCE] Original allocator entry forwards its real cleanup subobject;
 * two recursive lock acquisitions are preserved by the outer/inner wrappers. */
struct AsyncHeap {
    unsigned reserved00;
    struct StreamMutex mutex;
    struct CleanupContext allocator;
};

#ifdef __cplusplus
extern "C" {
#endif
void *func_001383e8(struct FrameHeap *heap, unsigned size, int alignment);
extern void *func_001d995c(struct CleanupContext *context, unsigned size,
                  CleanupCallback callback, void *cookie);
extern void func_001d9ca0(void *memory, unsigned size, void *cookie);
extern void *func_001d9ce4(struct AsyncHeap *heap, unsigned size);
#ifdef __cplusplus
}
#endif

#endif
