#include "game/functions.h"
#include "game/screen.h"

static struct DisplayQueueCounter *enable_state(void)
{
    return &bss_00495c70;
}

void func_0019eb4c(void)
{
    enable_state()->enabled = 1;
}
