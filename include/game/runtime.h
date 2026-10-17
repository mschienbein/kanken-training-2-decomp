#ifndef GAME_RUNTIME_H
#define GAME_RUNTIME_H

/* Process-wide helpers used across subsystems. */

typedef struct BufferState {
    void *buffer;
    unsigned int count;
    unsigned int capacity;
} BufferState;

struct Allocation {
    void *pointer;
    unsigned int count;
};

struct AllocationRecord {
    void *allocation;
    int count;
    int reserved[2];
};

struct MotionState {
    unsigned char reserved00[0x10];
    float scale;
    unsigned char reserved14[0x18];
    int count;
    void *allocation;
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

extern BufferState bss_0049da8c;
extern BufferState bss_0049cbc0;
extern struct Allocation bss_0049cbb4;
extern struct AllocationRecord bss_0049cb24[8];
extern int data_0024223c;
extern void *func_0011fa34(void *object);
extern void *func_0011ffdc(void *object);
extern void *func_00196cb8(int screen, int index);
extern void func_0019cf20(struct ResourceHandle *resource);
extern void func_0019dc20(void *allocation);
extern void func_001a000c(void *destination, const void *source, unsigned size);
extern void func_0011ef54(long long nanoseconds); /* sleep wrapper */
extern void func_0011ef58(long long nanoseconds); /* underlying sleep operation */
extern void *func_0013714c(void *object);      /* empty destructor wrapper */
extern void *func_00137150(void *object);      /* preserves the allocation pointer */
extern void func_001852f4(void *allocation);   /* releases a heap allocation */
extern void *func_0018f7f0(void *object);      /* preserves the allocation pointer */
extern void func_0019f568(void);               /* fatal-error / assertion handler */
extern void *func_001ef880(void *object);      /* array-element destroy callback */
extern void *func_001ef884(void *object);      /* preserves the element pointer */
extern void func_001ef888(void *allocation);   /* releases an array allocation */

#ifdef __cplusplus
}
#endif

#endif
