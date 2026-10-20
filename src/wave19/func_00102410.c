#include "game/scheduler.h"

/* */

void func_00102410(void)
{
    struct Scheduler *state = &bss_00495c8c;
    unsigned i;
    state->end = -1;
    state->begin = -1;
    state->current = -1;
    state->next = -1;
    state->pending = 0;
    for (i = 0; i < 64; ++i) {
        struct ScheduledTask *task = &state->tasks[i];
        task->enabled = 0;
        task->previous = -1;
        task->next = -1;
        task->priority = 0;
        task->binding.callback = 0;
        task->binding.wait = 0;
        task->repeat = 0;
        task->reserved14 = 0;
    }
}
