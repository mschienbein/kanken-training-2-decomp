#include "game/startup.h"
#include "game/scheduler.h"

static void install_callback(void (*callback)(void), struct ScheduledTask *task) {
    task->callback = callback;
    task->wait = 0;
}
int func_00141c74(void)
{
    if (bss_0049629c.suspended != 0) {
        if (bss_0049629c.suspended == 1) {
            if (bss_0049629c.callback) {
                bss_0049629c.callback();
                bss_0049629c.callback = 0;
            }
            install_callback(func_0010433c, &bss_00495c8c.tasks[0]);
            return 1;
        }
        --bss_0049629c.suspended;
    }
    return 0;
}
