#ifndef GAME_RUNTIME_H
#define GAME_RUNTIME_H
#include <stddef.h>
#include "game/input.h"

/* Process-wide helpers used across subsystems. */

typedef struct BufferState {
    void *buffer;
    unsigned int count;
    unsigned int capacity;
} BufferState;

struct TaggedBufferState {
    BufferState allocation;
    unsigned state;
};
struct ExtendedBufferState {
    BufferState allocation;
    unsigned state;
    unsigned first, second;
};
struct BufferOwner {
    unsigned char reserved[0xc08];
    void *buffer;
    unsigned count, owned, state, active;
};

struct ByteWriter { unsigned char *data; unsigned position; };
struct Allocation {
    void *pointer;
    unsigned int count;
};

/* The allocation and optional-value fields share the address-rooted owner. */
struct OptionalAllocation {
    struct Allocation allocation;
    int value;
    unsigned reserved0c;
    unsigned state;
};

struct AllocationRecord {
    void *allocation;
    int count;
    int reserved[2];
};

struct AllocationRecordTable {
    struct AllocationRecord records[8];
    unsigned reserved80[2];
    unsigned mode;
};

struct BitmapPool {
    unsigned char *storage;
    unsigned int reserved04;
    unsigned int stride;
    int capacity;
    int used;
#ifndef __cplusplus
    unsigned char bits[];
#endif
};

struct MotionKey {
    float time;
    unsigned flags;
    unsigned char reserved08[0x0c];
    float value;
};
struct MotionState {
    struct InputMode mode;
    unsigned key_flags;
    unsigned flags;
    float time;
    float scale;
    float first_time;
    float last_time;
    int first;
    int last;
    int current;
    float blend;
    int count;
    struct MotionKey *allocation;
};

struct ResourceReleasable;
struct ResourceHandle {
    unsigned active;
    unsigned char reserved04[0x0c];
    struct ResourceReleasable *object;
    unsigned char reserved14[0x44];
    void *allocation;
    unsigned char reserved5c[8];
    int kind;
};

#ifdef __cplusplus
void *operator new(size_t size, const void *tag) throw();
extern "C" {
#endif

/* ARM EABI's four-byte-aligned copy helper; the bound identity is already proven. */
extern void __aeabi_memcpy4(void *destination, const void *source, size_t size);
extern void func_00192720(void *destination, const void *source, unsigned size);
extern int func_00122350(int index);
extern const char *func_0012142c(unsigned region);
extern int func_0012251c(char *destination, const char *format, ...);
extern void *func_0019df08(void *destination, unsigned size);
extern void func_0016ee38(struct MotionState *state);
extern int func_0016ece8(struct MotionState *state, const struct MotionKey *keys, int heap);
extern void func_0016ec84(struct MotionState *state, int first, int last);
extern void func_0016ec6c(struct MotionState *state, int looping);
extern void func_0016ec28(struct MotionState *state, int frame);
extern void func_0016e924(struct MotionState *state, float delta);
extern const struct MotionKey data_002188e8[4];
extern const struct MotionKey data_002180fc[3];
extern unsigned char *func_00152d48(struct BitmapPool *pool);
extern void func_0017e5f4(struct BitmapPool *pool, const void *element);
extern struct BufferOwner bss_002d4270;
extern struct ExtendedBufferState bss_0049da8c;
extern struct TaggedBufferState bss_0049cbc0;
extern struct Allocation bss_0049cbb4;
extern struct OptionalAllocation bss_002d2138;
extern struct AllocationRecordTable bss_0049cb24;
extern int data_0024223c;
extern const char data_002232d5[];
extern void *func_0019cff8(unsigned region);
extern void *func_00190ae0(unsigned region);
extern void *func_00193110(unsigned size, const char *tag);
extern void *func_0019ffa4(unsigned size, const void *tag);
extern void *func_0011fa34(void *object);
extern void *func_0011ffdc(void *object);
extern void *func_00196cb8(int screen, int index);
extern void func_0019cf20(struct ResourceHandle *resource);
extern void func_0019dc20(void *allocation);
extern void func_0011ef54(long long nanoseconds); /* sleep wrapper */
extern void func_0011ef58(long long nanoseconds); /* underlying sleep operation */
extern void *func_0013714c(void *object);      /* empty destructor wrapper */
extern void *func_00137150(void *object);      /* preserves the allocation pointer */
extern void func_001852f4(void *allocation);   /* releases a heap allocation */
extern void func_0019e08c(void *allocation);
extern void *func_0018f7f0(void *object);      /* preserves the allocation pointer */
extern void func_0019d6c4(unsigned result, unsigned instruction_address);
extern int func_0019f568(void);               /* fatal-error / assertion handler */
extern void *func_001ef880(void *object);      /* array-element destroy callback */
extern void *func_001ef884(void *object);      /* preserves the element pointer */
extern void func_001ef888(void *allocation);   /* releases an array allocation */
/* Runtime __ARM_common_memclr4_8; callers do not consume a C return value. */
extern void func_00130f90(void *destination);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
extern "C" {
#endif
void func_0019dc1c(void *allocation);
extern void func_001bbc0c(void *allocation);
#ifdef __cplusplus
}
#endif

#endif
