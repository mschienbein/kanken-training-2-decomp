#include "game/functions.h"
#include "game/scheduler.h"

/* C-only procedure: callers MUST discard its unspecified result (6.9.1p12). */
int func_00186944(int index, int value)
{
    bss_00495c8c.tasks[index].binding.wait = value;
}
