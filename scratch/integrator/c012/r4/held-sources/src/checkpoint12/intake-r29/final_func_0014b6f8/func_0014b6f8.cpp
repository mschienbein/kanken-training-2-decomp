#include "game/task_dispatcher.h"

/* [INFERENCE] Source-local acquisition result, not a recovered external type. */
struct AcquiredTask {
    StreamMutex *lock;
    AudioWorkRequest *task;
};

static AcquiredTask acquire_first_task(AudioTaskManager *dispatcher)
{
    AcquiredTask acquired;
    acquired.lock = &dispatcher->lock;
    func_00192bd8(acquired.lock);
    acquired.task = func_0017f504(dispatcher, 2, 1);
    return acquired;
}

extern "C" void func_0014b6f8(struct AudioTaskManager *dispatcher)
{
    AcquiredTask acquired = acquire_first_task(dispatcher);
    StreamMutex *lock = acquired.lock;
    AudioWorkRequest *task = acquired.task;
    AudioWorkRequest *idle = 0;
    if (task != idle) {
execute:
        func_00195d74(lock);
        dispatcher->current = task;
        task->type->execute(task);
        task->active = 3;
        func_0019ea6c(&task->completion);
        dispatcher->current = idle;
        return;
    }
    task = func_0017f504(dispatcher, 1, 1);
    if (task != 0) goto execute;
    task = func_0017f504(dispatcher, 0, 1);
    if (task != 0) goto execute;
    func_00195d74(lock);
}
