#include "game/command_group.h"
#include "game/pool_list.h"


/* The try-wait operation consumes the event; the argument is writable. */

int func_001d85d8(struct PendingTask128 *task)
{
    if (func_0019ec14(&task->completion))
        return 0;
    return 1;
}
