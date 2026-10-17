#ifndef GAME_STREAM_MANAGER_H
#define GAME_STREAM_MANAGER_H

struct RenderResource;
struct ResourceCommand;
struct X03ServiceReferences;

#include "ctr/svc.h"

#include <stdbool.h>
#include "game/functions.h"

struct StreamEvent {
    struct StreamEvent *next;
    unsigned char opcode;
    unsigned char reserved05[3];
    unsigned int serial;
    unsigned int start;
    void *object;
};

struct StreamSemaphore {
    int value;
    short waiters;
    short limit;
};

struct StreamCriticalSection {
    unsigned int state;
};

struct StreamMutex {
    struct StreamCriticalSection critical;
    void *owner;
    unsigned int depth;
};

struct StreamQueue {
    struct StreamEvent **slots;
    struct StreamSemaphore readable;
    struct StreamSemaphore available;
    struct StreamMutex mutex;
    unsigned int capacity;
    unsigned int read_index;
    int count;
    unsigned int field2c;
    int waiting_writers;
};

struct StreamManager {
    struct StreamQueue requests;
    struct StreamEvent *request_storage[32];
    struct StreamQueue completions;
    struct StreamEvent *completion_storage[33];
    struct StreamEvent *pending_head;
    struct StreamEvent *pending_tail;
    unsigned int serial;
    unsigned int completed_serial;
    unsigned int *buffer;
    unsigned int capacity;
    unsigned int write_index;
    unsigned int read_index;
    bool full;
    unsigned char initialized;
    unsigned char reserved18e[2];
};

struct StreamManagerGuards {
    volatile unsigned int first;
    volatile unsigned int second;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct StreamManagerGuards data_0024371c;
/* [INFERENCE] The image references this same volatile guard both as parent+4
 * and as a direct symbol. This names the second word, not another allocation. */
extern volatile unsigned int data_00243720;
extern struct StreamManager bss_004b8640;
extern struct StreamManager bss_004b87d0;
extern void func_00146ff8(struct StreamManager *manager, void *storage, unsigned int size);
extern void func_00180884(struct StreamQueue *queue, struct StreamEvent **storage,
                          unsigned int capacity);
extern struct StreamManager *func_00143d1c(struct StreamManager *manager);
extern void func_001812f8(struct StreamManager *manager);
extern int func_00181280(struct StreamSemaphore *semaphore, int amount);
extern struct StreamManager *func_001916e8(void);
extern void *func_0019147c(struct StreamManager *manager, unsigned words);
extern unsigned func_00191458(struct StreamManager *manager, struct StreamEvent *event);
extern unsigned func_001912b0(struct StreamManager *manager, int wait);
extern void func_0019124c(struct StreamManager *manager, unsigned serial);
extern struct StreamEvent *func_001829e0(struct StreamQueue *queue);
extern void func_00192bd8(struct StreamMutex *mutex);
extern void func_00195d74(struct StreamMutex *mutex);
extern struct StreamManager *func_00185bfc(void);
extern void func_00131284(struct StreamManager *manager);
extern struct StreamQueue *func_00183f18(struct StreamQueue *queue);
extern int func_001837c8(struct StreamQueue *queue, struct StreamEvent **output);
#ifdef __cplusplus
}
#endif


struct RenderResource {
    unsigned char reserved00[0x218];
    unsigned allocation;
    unsigned count;
};

#ifdef __cplusplus
struct ResourceCommand : StreamEvent {
    StreamEvent *initialize(RenderResource *resource)
    {
        opcode = 0x33;
        object = resource;
        return this;
    }
};
#endif

struct X03ServiceReferences {
    unsigned char initialized;
    unsigned char reserved[7];
    int references;
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct X03ServiceReferences data_00243048;
extern CtrHandle data_00243054;
extern const CtrHandle data_0021c5d0;
extern struct StreamMutex *func_0018f734(void);
extern void func_00124aac(struct RenderResource *resource);
extern void func_001217b8(void);
#ifdef __cplusplus
}
#endif

#endif
