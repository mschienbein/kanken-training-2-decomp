#include "game/resource_runtime.h"
/* [INFERENCE] Only zero initialization and the first-word readiness query
 * establish these fields' roles; the remaining words are unnamed state. */

void func_00105420(void)
{
    bss_00495c5c.ready = 0;
    bss_00495c5c.state04 = 0;
    bss_00495c5c.state08 = 0;
    bss_00495c5c.state0c = 0;
    bss_00495c5c.value = 0;
}
