#include <stddef.h>
#include "game/synchronization.h"
#include "game/task_dispatcher.h"



extern "C" void func_00149270(struct WorkerThread *worker)
{
    while (!worker->stop_requested) {
        func_0014b7a0(func_00185de4());
        if (worker->stop_requested)
            break;
        struct StreamMutex *lock = &worker->lock;
        func_00192bd8(lock);
        func_0014b6f8(func_00185de4());
        func_00195d74(lock);
        func_00140904(func_00185bfc());
    }
}
