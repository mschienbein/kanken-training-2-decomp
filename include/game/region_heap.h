#ifndef GAME_REGION_HEAP_H
#define GAME_REGION_HEAP_H

#include "game/object_manager.h"

struct FreeLink {
    struct FreeLink *next;
};

struct RegionList {
    unsigned reserved00[2];
    struct FreeLink *head;
};

struct RegionBlock;
struct RegionHeap {
    const void *vtable;
    struct PoolLink registration;
    unsigned char *start;
    unsigned char *end;
    unsigned signature;
    struct RegionHeap *parent;
    struct PoolLink children;
    unsigned options;
    unsigned char *low;
    unsigned char *high;
    struct RegionBlock *blocks;
};

struct HeapStorage {
    struct RegionHeap *heap;
    struct PoolList pending;
#ifdef __cplusplus
    ~HeapStorage();
    int initialize(void *memory, unsigned size);
#endif
};

struct RegionHeapOwner {
    const void *vtable;
    struct StreamMutex lock;
    struct HeapStorage storage;
};


#ifdef __cplusplus
extern "C" {
#endif
extern int func_00125a5c(struct HeapStorage *storage);
extern int func_0013f110(struct RegionHeap *heap, unsigned tag);
extern int func_0013f06c(struct RegionHeap *heap, unsigned tag);
/* [INFERENCE] The observed callers ignore the allocator-mode operation's
 * result; the pointer-returning contract retains the delivered heap view. */
extern struct RegionHeap *func_00146758(struct RegionHeap *heap, unsigned flags);
extern void func_001467e8(struct RegionHeap *heap);
extern void func_001834b8(struct HeapStorage *storage);
extern struct RegionHeap *func_001834a4(struct RegionHeap *heap);
extern int func_001855d8(struct HeapStorage *storage);
extern void func_001855fc(struct HeapStorage *storage, int count);
extern int func_001856d8(struct HeapStorage *storage);
extern void func_00183724(struct FreeLink **head, void *storage, unsigned size);
#ifdef __cplusplus
}
#endif

#endif
