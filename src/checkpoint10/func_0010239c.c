#include "game/scheduler.h"

void func_0010239c(void)
{
    int index = bss_00495c8c.end;
    if (index == -1)
        return;
    do {
        struct ScheduledTask *task;
        bss_00495c8c.current = index;
        task = &bss_00495c8c.tasks[index];
        bss_00495c8c.next = task->previous;
        if (task->enabled &&
            ((!bss_00495c8c.pending || !task->repeat) &&
             task->enabled == 1)) {
            if (task->binding.callback)
                task->binding.callback();
        }
        index = bss_00495c8c.next;
    } while (index != -1);
}
