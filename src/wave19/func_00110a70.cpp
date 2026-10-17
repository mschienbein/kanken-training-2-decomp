#include "game/scheduler.h"



static void set_task_callback(struct ScheduledTask *task,
                              int (*callback)(void), int wait)
{
    task->binding.callback = callback;
    task->binding.wait = wait;
}

void func_00110a70(void)
{
    set_task_callback(&bss_00495c8c.tasks[bss_004990d0.current],
                      func_001900a8, 0);
}
