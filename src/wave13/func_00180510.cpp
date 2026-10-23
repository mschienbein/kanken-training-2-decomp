#include "game/cleanup.h"
#include "game/sound_task.h"
#include "game/synchronization.h"
#include "game/stream_manager.h"




extern "C" SoundTaskBase *func_00180510(SoundTaskBase *task)
{
    task->vtable = &data_00224890;
    task->link.next = 0;
    task->link.previous = 0;
    task->state.state = 0;
    task->state.critical.state = 0;
    func_0019df28(&task->state, 1);
    task->active = false;
    task->owner = 0;
    func_0019ea6c(&task->state);
    return task;
}
