#include "game/scheduler.h"



static void set_task_callback(struct ScheduledTask *task,
                              void (*callback)(void), int wait)
{
    task->binding.callback = callback;
    task->binding.wait = wait;
}

void func_00110a70(void)
{
    /* The exact scheduler slot consumer discards the callback's int result. */
    set_task_callback(&bss_00495c8c.tasks[bss_004990d0.current],
                      (void (*)(void))func_001900a8, 0);
}
