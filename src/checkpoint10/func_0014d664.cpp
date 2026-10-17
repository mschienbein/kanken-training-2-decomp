#include "game/sound_task.h"

struct SoundQueuedTask {
    SoundTaskBase base;
    unsigned arguments[11];
    unsigned char option;
    unsigned char reserved49[3];
    void *source;
    PoolLink queue_link;
};

/* INTEGRATOR: move to include/game/sound_task.h */
extern "C" {
extern const unsigned data_00224978[];
}

extern "C" SoundQueuedTask *func_0014d664(SoundQueuedTask *task)
{
    task = (SoundQueuedTask *)func_00180510(&task->base);
    task->base.vtable = data_00224978;
    PoolLink *link = &task->queue_link;
    link->next = 0;
    link->previous = 0;
    return task;
}
