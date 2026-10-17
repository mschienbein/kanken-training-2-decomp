#ifndef GAME_EXP_HEAP_H
#define GAME_EXP_HEAP_H

#include "game/stream_manager.h"

/* [INFERENCE] SDK expandable-heap names. The intrusive-list offsets, bounds,
 * halfword group/direction and signed mode byte are independently accessed. */
struct IntrusiveList { void *head; void *tail; unsigned short count, link_offset; };
struct IntrusiveLink { void *previous; void *next; };
struct HeapBase {
    unsigned signature;
    struct IntrusiveLink link;
    struct IntrusiveList children;
    unsigned char *begin;
    unsigned char *end;
    unsigned attributes;
};
struct HeapBlock {
    unsigned short signature, attributes;
    unsigned size;
    struct HeapBlock *previous, *next;
};
struct ExpHeapData {
    struct HeapBlock *free_head, *free_tail, *used_head, *used_tail;
    unsigned short group;
    unsigned short direction : 1;
    unsigned short reserved_flags : 15;
    signed char mode;
    unsigned char reserved15[3];
};
struct ExpHeap { struct HeapBase base; struct ExpHeapData data; };
struct ManagedExpHeap {
    unsigned char reserved00[0x18];
    struct ExpHeap heap;
    unsigned allocations;
};
struct LockedExpHeap { struct ManagedExpHeap base; struct StreamMutex mutex; };
struct HeapArena {
    unsigned char reserved00[0x18];
    struct ExpHeap heap;
    unsigned allocations;
    unsigned char *lower;
    unsigned size;
};
struct StreamLockGuard { struct StreamMutex *mutex; };

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned short func_0012ee34(struct ExpHeap *heap, int group);
extern int func_0012ee44(struct ExpHeap *heap, int direction);
extern int func_0012ef20(struct ExpHeap *heap, int mode);
extern void *func_0012ec8c(struct ExpHeap *heap, unsigned size, int alignment);
extern struct ExpHeap *func_001219f4(struct ExpHeap *heap, void *memory,
                                    unsigned size, unsigned short attributes);
extern void *func_00195104(struct ManagedExpHeap *heap, unsigned size, int alignment,
                          int group, int direction, int mode);
extern void *func_0019ffa8(unsigned size);
extern void *func_0018627c(const struct IntrusiveList *list, const void *object);
extern struct LockedExpHeap *data_00242c54;
#ifdef __cplusplus
}
#endif
#endif
