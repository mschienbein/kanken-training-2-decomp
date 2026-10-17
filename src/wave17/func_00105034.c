#include "game/resource_runtime.h"
/* [INFERENCE] Only zero initialization and the first-word readiness query
 * establish these fields' roles; the remaining words are unnamed state. */

void func_00105034(void)
{
    bss_00495c48.ready = 0;
    bss_00495c48.state04 = 0;
    bss_00495c48.state08 = 0;
    bss_00495c48.state0c = 0;
    bss_00495c48.value = 0;
}
