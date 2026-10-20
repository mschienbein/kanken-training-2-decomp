#include "game/scheduler.h"
#include "game/startup.h"

/* This is task zero's existing binding, not a second UpdateState object. */
/* C-only procedure: callers MUST discard its unspecified result (6.9.1p12). */
int func_00107a88(ScheduledTaskCallback callback)
{
    bss_00495c8c.tasks[0].binding.callback = callback;
    bss_00495c8c.tasks[0].binding.wait = 0;
}
