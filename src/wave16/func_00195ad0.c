#include "game/scheduler.h"

void func_00195ad0(unsigned int index)
{
    struct ScheduledTask *task = &bss_00495c8c.tasks[index];

    if (task->enabled) {
        task->enabled = 0;
        if (bss_00495c8c.next == index)
            bss_00495c8c.next = task->previous;
        if (task->next >= 0)
            bss_00495c8c.tasks[task->next].previous = task->previous;
        if (task->previous >= 0)
            bss_00495c8c.tasks[task->previous].next = task->next;
        if (task->previous < 0)
            bss_00495c8c.begin = task->next;
        if (task->next < 0)
            bss_00495c8c.end = task->previous;
    }
}
