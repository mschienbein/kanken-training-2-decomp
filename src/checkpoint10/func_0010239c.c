#include "game/scheduler.h"
/* INTEGRATOR: differs from include/game/scheduler.h: the dispatcher loads
 * enabled as a signed halfword and reads a scheduling-mode word at 0x60c. */
struct ScheduledTask {
    short enabled;
    short previous, next;
    unsigned short priority;
    void (*callback)(void);
    int wait;
    int repeat;
    int reserved14;
};
struct Scheduler {
    struct ScheduledTask tasks[64];
    int current;
    int next;
    short end, begin;
    int dispatch_mode;
};


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
            ((!bss_00495c8c.dispatch_mode || !task->repeat) &&
             task->enabled == 1)) {
            if (task->callback)
                task->callback();
        }
        index = bss_00495c8c.next;
    } while (index != -1);
}
