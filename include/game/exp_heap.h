#ifndef GAME_EXP_HEAP_H
#define GAME_EXP_HEAP_H
#include <stdarg.h>

struct AllocatorInterface;
struct FormattedText;
struct G103798;
struct ResourceEntry;
struct ResourceObject;
struct RuntimeTypeInfo;
struct RuntimeTypedObject;
struct StreamMutex;


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
struct StreamLockGuard {
    struct StreamMutex *mutex;
#ifdef __cplusplus
    explicit StreamLockGuard(struct StreamMutex *value) : mutex(value) { func_00192bd8(mutex); }
    ~StreamLockGuard() { func_00195d74(mutex); }
#endif
};;

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

struct G103798 { void *p; int n; };

struct ResourceEntry {
    int active;
    void *allocation;
    struct ResourceObject *object;
    unsigned char reserved[3];
    unsigned char flag;
    unsigned char tail[8];
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct G103798 data_00242220;
extern void *func_0019d798(int heap, int alignment, unsigned size);
extern void func_001852f8(void *allocation);
extern int func_0010083c(unsigned short *buffer, unsigned count,
                         const unsigned short *format, va_list arguments);
extern struct ResourceEntry *func_0015d798(struct ResourceEntry *entry, int screen,
    unsigned char flag, float x, float y, float z, unsigned color,
    unsigned capacity, const unsigned short *text);
extern void func_00121a88(struct ExpHeap *heap, void *allocation);
extern struct HeapArena bss_00495ad0;
extern void func_00193a6c(struct HeapArena *heap, void *allocation);
extern struct RuntimeTypeInfo data_00242da8;
extern struct RuntimeTypeInfo data_00242e2c;
extern struct RuntimeTypeInfo data_00243494;
extern struct RuntimeTypeInfo data_00242da4;
extern struct RuntimeTypeInfo data_00242e24;
extern unsigned short func_001f15e0(const struct FormattedText *text);
extern void func_001037c0(void);
extern void func_00110eac(unsigned count);
extern void func_00110ed8(int count);
extern struct ResourceEntry *func_0015db7c(int screen, unsigned char flag,
    float x, float y, float z, unsigned color, unsigned capacity,
    const unsigned short *format, ...);
extern void func_0015e3b4(void);
extern void func_001e37e0(struct AllocatorInterface *interface, void *allocation);
extern struct RuntimeTypeInfo *func_001f014c(const struct RuntimeTypedObject *object);
extern struct RuntimeTypeInfo *func_001f0158(const struct RuntimeTypedObject *object);
extern struct RuntimeTypeInfo *func_001f017c(const struct RuntimeTypedObject *object);
extern struct RuntimeTypeInfo *func_001f0188(const struct RuntimeTypedObject *object);
extern struct RuntimeTypeInfo *func_001f0194(const struct RuntimeTypedObject *object);
#ifdef __cplusplus
}
#endif

#endif
