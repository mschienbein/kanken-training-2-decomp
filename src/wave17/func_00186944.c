#include "game/functions.h"
#include "game/scheduler.h"

void func_00186944(int index, int value)
{
    bss_00495c8c.tasks[index].binding.wait = value;
}
