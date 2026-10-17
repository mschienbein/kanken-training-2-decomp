#ifndef GAME_WAITABLE_HANDLE_H
#define GAME_WAITABLE_HANDLE_H
#include "ctr/svc.h"

/* The common wait-once prefix is read at +0 and +4 by func_0019df84. */
struct WaitableHandle {
    CtrHandle handle;
    unsigned char waited;
    unsigned char reserved05[3];
};
struct WorkerThread {
    struct WaitableHandle thread;
    unsigned char reserved08[8];
    int thread_id;
    unsigned char stop_requested;
    unsigned char running;
};
struct LargeResourceElement { unsigned char reserved00[0x2f4f4]; };
struct LargeHandleBlock {
    struct WaitableHandle completion;
    unsigned char reserved08[0x1018];
};
struct LargeHandleOwner {
    unsigned char reserved00[0x10c];
    struct LargeResourceElement elements[1];
    unsigned char reserved2f600[0xd4];
    struct LargeHandleBlock handles[3];
};
#ifdef __cplusplus
extern "C" {
#endif
extern void func_0019df84(struct WaitableHandle *completion);
extern void func_0019dfbc(CtrResult result);
extern void func_00185d74(struct WorkerThread *worker);
extern struct WorkerThread *func_00143ac8(struct WorkerThread *worker);
extern struct WaitableHandle *func_001c52bc(struct WaitableHandle *object);
extern struct LargeResourceElement *func_001c3100(struct LargeResourceElement *element);
extern void func_0019f6b4(struct LargeResourceElement *array,
    struct LargeResourceElement *(*destroy)(struct LargeResourceElement *),
    unsigned int element_size, unsigned int count);
#ifdef __cplusplus
}
#endif
#endif
