#include "game/sound_task.h"
#include <stddef.h>


extern "C" void *func_0014d664(void *storage)
{
    SoundQueuedTask *task = static_cast<SoundQueuedTask *>(storage);
    /* The returned pointer denotes the actual first member of this object. */
    task = reinterpret_cast<SoundQueuedTask *>(func_00180510(&task->base));
    task->base.vtable = data_00224978;
    PoolLink *link = &task->queue_link;
    link->next = 0;
    link->previous = 0;
    return task;
}
