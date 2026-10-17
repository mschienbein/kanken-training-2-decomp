#ifndef GAME_TASK_DISPATCHER_H
#define GAME_TASK_DISPATCHER_H

#include <stddef.h>
#include "game/synchronization.h"

#ifdef __cplusplus
/* [INFERENCE] Descriptive types and callback roles follow the matching consumers. */
class QueuedTask;
struct TaskDispatcher;
struct DispatcherWorker;

class QueuedTask {
public:
    virtual ~QueuedTask();
    virtual void execute();
    struct PoolLink link;
    struct SignalState completion;
    unsigned char state;
};
struct TaskDispatcher {
    struct PoolList queues[3];
    QueuedTask *current;
    unsigned char active;
    unsigned char reserved29[3];
    struct StreamMutex lock;
    struct StreamQueue wakeups;
};
struct DispatcherWorker {
    unsigned char reserved00[8];
    struct StreamMutex lock;
    unsigned char stopped;
};

extern "C" {
extern struct TaskDispatcher *func_00185de4(void);
extern void func_0014b7a0(struct TaskDispatcher *dispatcher);
extern void func_0014b6f8(struct TaskDispatcher *dispatcher);
extern struct StreamManager *func_00185bfc(void);
extern void func_00140904(struct StreamManager *manager);
extern QueuedTask *func_0017f504(struct TaskDispatcher *dispatcher, int priority,
                                int remove);
extern struct StreamEvent *func_001829e0(struct StreamQueue *queue);
}
#endif

#endif
