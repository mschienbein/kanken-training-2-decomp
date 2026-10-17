#include <stddef.h>
#include "game/synchronization.h"
#include "game/task_dispatcher.h"



extern "C" void func_0014b7a0(struct TaskDispatcher *dispatcher)
{
    dispatcher->active = 0;
    while (!func_0017f504(dispatcher, 2, 0) &&
           !func_0017f504(dispatcher, 1, 0) &&
           !func_0017f504(dispatcher, 0, 0) && !dispatcher->active) {
        if (!func_001829e0(&dispatcher->wakeups))
            break;
    }
}
