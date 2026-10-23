#ifndef GAME_REGION_HEAP_H
#define GAME_REGION_HEAP_H

struct HeapLink;
struct HeapList;
struct PoolLink;
struct PoolList;
struct StreamMutex;

#include "game/pool_list.h"

#include "game/stream_manager.h"

struct FreeLink {
    struct FreeLink *next;
};

struct RegionList {
    unsigned reserved00[2];
    struct FreeLink *head;
};

/* The native FrameHeap state chain stores both allocation boundaries. */
struct RegionBlock {
    unsigned tag;
    unsigned char *low;
    unsigned char *high;
    struct RegionBlock *previous;
};
struct RegionHeap {
    const void *vtable;
    struct PoolLink registration;
    unsigned char *start;
    unsigned char *end;
    unsigned signature;
    struct PoolList children;
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
    void clear();
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
/* [INFERENCE] Candidate-only release hooks; no historical identity/address is proved. */
void frame_heap_release_lock(void *heap);
void frame_heap_release_unlock(void *heap);
extern int func_00125a5c(struct HeapStorage *storage);
extern int func_00116ce8(struct RegionHeapOwner *owner);
extern void func_0017f4f4(struct FreeLink **head, struct FreeLink *node);
extern int func_0013f110(struct RegionHeap *heap, unsigned tag);
extern int func_0013f06c(struct RegionHeap *heap, unsigned tag);
/* Native FrameHeap::Free(int) and every recovered consumer discard r0.
 * The operation has no pointer result; the C view retains the full mode word. */
extern void func_00146758(struct RegionHeap *heap, unsigned flags);
/* [INFERENCE] The return-only hook's callers retain the heap owner input,
 * although erased v2 calls do not establish the original helper's identity. */
extern void func_001467e4(struct RegionHeap *heap);
extern void func_001467e8(struct RegionHeap *heap);
extern void func_001834b8(struct HeapStorage *storage);
extern struct RegionHeap *func_001834a4(struct RegionHeap *heap);
extern int func_001855d8(struct HeapStorage *storage);
/* Call sites 0x00125a7c and 0x00147644 consume r0; signed spelling [INFERENCE]. */
extern int func_001855fc(struct HeapStorage *storage, int count);
extern int func_001856d8(struct HeapStorage *storage);
extern void func_00183724(struct FreeLink **head, void *storage, unsigned size);
#ifdef __cplusplus
}
#endif


struct HeapLink {
    struct HeapLink *next;
    struct HeapLink *previous;
};

struct HeapList {
    unsigned count;
    struct HeapLink root;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct RegionHeap *func_00148b08(void *memory, unsigned size, unsigned options);
#ifdef __cplusplus
}
#endif

#endif
