#include "game/command_group.h"
#include "game/pool_list.h"


/* The callee links the writable queued object into a context-owned list. */

void func_001d85f4(struct PendingTask128 *task)
{
    if (task->queued)
        func_0016a0d0(task->context, task->queued);
}
