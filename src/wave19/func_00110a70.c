#include "game/scheduler.h"

static void set_task_callback(struct ScheduledTaskBinding *binding,
                              ScheduledTaskCallback callback, int wait)
{
    binding->callback = callback;
    binding->wait = wait;
}

/* C-only procedure callback: every caller discards its value (C11 6.9.1p12).
 * Do not consume the result or compile this definition as C++. */
int func_00110a70(void)
{
    set_task_callback(&bss_00495c8c.tasks[bss_004990d0.current].binding,
                      func_001900a8, 0);
}
