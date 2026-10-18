#ifndef GAME_WAITABLE_HANDLE_H
#define GAME_WAITABLE_HANDLE_H

struct JoinedHandleOwner10;
struct JoinedHandleOwner14;
struct JoinedHandleOwner18;
struct JoinedHandleOwnerC20;

#include "ctr/svc.h"
#include "game/stream_manager.h"

/* The common wait-once prefix is read at +0 and +4 by func_0019df84. */
struct WaitableHandle {
    CtrHandle handle;
    unsigned char waited;
    unsigned char attached; /* [INFERENCE] Initialized alongside waited at +5. */
    unsigned char reserved06[2];
};
struct WorkerThread {
    struct WaitableHandle thread;
    struct StreamMutex lock;
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
extern void func_00149270(struct WorkerThread *worker);
extern struct WorkerThread *func_00143ac8(struct WorkerThread *worker);
extern struct WaitableHandle *func_001c52bc(struct WaitableHandle *object);
extern struct LargeResourceElement *func_001c3100(struct LargeResourceElement *element);
extern void func_0019f6b4(struct LargeResourceElement *array,
    struct LargeResourceElement *(*destroy)(struct LargeResourceElement *),
    unsigned int element_size, unsigned int count);
#ifdef __cplusplus
}
#endif

struct JoinedHandleOwner14 {
    unsigned char reserved00[0x14];
    struct WaitableHandle completion;
};

struct JoinedHandleOwner18 {
    unsigned char reserved00[0x18];
    struct WaitableHandle completion;
};

struct JoinedHandleOwner10 {
    unsigned char reserved00[0x10];
    struct WaitableHandle completion;
};

struct JoinedHandleOwnerC20 {
    unsigned char reserved00[0xc20];
    struct WaitableHandle completion;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct JoinedHandleOwner14 *func_001b7308(struct JoinedHandleOwner14 *object);
extern struct JoinedHandleOwner18 *func_001bba58(struct JoinedHandleOwner18 *object);
extern struct JoinedHandleOwner10 *func_001c0e48(struct JoinedHandleOwner10 *object);
extern struct JoinedHandleOwnerC20 *func_001c2158(struct JoinedHandleOwnerC20 *object);
#ifdef __cplusplus
}
#endif

#endif
