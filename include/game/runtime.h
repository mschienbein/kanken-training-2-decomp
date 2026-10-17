#ifndef GAME_RUNTIME_H
#define GAME_RUNTIME_H

/* Process-wide helpers used across subsystems. */

typedef struct BufferState {
    void *buffer;
    unsigned int count;
    unsigned int capacity;
} BufferState;

struct ByteWriter { unsigned char *data; unsigned position; };
struct Allocation {
    void *pointer;
    unsigned int count;
};

struct AllocationRecord {
    void *allocation;
    int count;
    int reserved[2];
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
    unsigned char reserved08[0x10];
};
struct MotionState {
    int mode;
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
extern "C" {
#endif

extern void func_00192720(void *destination, const void *source, unsigned size);
extern void func_0016ee38(struct MotionState *state);
extern int func_0016ece8(struct MotionState *state, const struct MotionKey *keys, int heap);
extern void func_0016ec84(struct MotionState *state, int first, int last);
extern void func_0016ec6c(struct MotionState *state, int looping);
extern void func_0016ec28(struct MotionState *state, int frame);
extern void func_0016e924(struct MotionState *state, float delta);
extern const struct MotionKey data_002188e8[4];
extern unsigned char *func_00152d48(struct BitmapPool *pool);
extern void func_0017e5f4(struct BitmapPool *pool, const void *element);
extern BufferState bss_0049da8c;
extern BufferState bss_0049cbc0;
extern struct Allocation bss_0049cbb4;
extern struct AllocationRecord bss_0049cb24[8];
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
extern void *func_0018f7f0(void *object);      /* preserves the allocation pointer */
extern void func_0019d6c4(unsigned result, unsigned instruction_address);
extern void func_0019f568(void);               /* fatal-error / assertion handler */
extern void *func_001ef880(void *object);      /* array-element destroy callback */
extern void *func_001ef884(void *object);      /* preserves the element pointer */
extern void func_001ef888(void *allocation);   /* releases an array allocation */
/* Runtime __ARM_common_memclr4_8; callers do not consume a C return value. */
extern void func_00130f90(void *destination);

#ifdef __cplusplus
}
#endif

#endif
