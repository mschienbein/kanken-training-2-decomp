#include "game/startup.h"

/* [INFERENCE] This external C procedure occupies a real int(void) scheduler
 * slot. Its result is unspecified and MUST be discarded; no definition or
 * historical return meaning is asserted here. */
extern int func_00110cc8(void);

void func_00107f0c(void)
{
    func_0019f338(3, func_00110cc8, 3);
    bss_00495c8c.tasks[3].repeat = 0;
}

/* [INFERENCE] Descriptive list roles follow the canonical scheduler links.
 * This genuine mutation body is C, not a visibility stub. Its unspecified
 * result MUST be discarded, and it must never be compiled as C++. */
int func_0019f338(int index, ScheduledTaskCallback callback, unsigned priority)
{
    struct Scheduler *scheduler = &bss_00495c8c;
    struct ScheduledTask *task = &scheduler->tasks[index];
    if (!task->enabled) {
        task->enabled = 1;
        task->priority = (unsigned short)priority;
        task->reserved14 = 0;
        task->binding.callback = callback;
        task->binding.wait = 0;
        task->repeat = 1;
        short slot = (short)index;
        struct ScheduledTask *position = 0;
        int cursor = scheduler->begin;
        if (scheduler->end != -1) {
            while (cursor != -1) {
                position = &scheduler->tasks[cursor];
                if (position->priority <= priority)
                    break;
                cursor = position->next;
            }
            if (cursor != -1) {
                task->previous = position->previous;
                task->next = (short)cursor;
                if (position->previous != -1)
                    scheduler->tasks[position->previous].next = slot;
                else
                    scheduler->begin = slot;
                position->previous = slot;
                if (scheduler->current == cursor)
                    scheduler->next = slot;
            } else {
                task->next = -1;
                task->previous = (short)(position - scheduler->tasks);
                position->next = slot;
                scheduler->end = slot;
            }
        } else {
            task->previous = -1;
            task->next = -1;
            scheduler->end = slot;
            scheduler->begin = slot;
        }
    }
}
