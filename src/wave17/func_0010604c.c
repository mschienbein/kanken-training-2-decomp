#include "game/resource_runtime.h"
/* [INFERENCE] Only zero initialization and the first-word readiness query
 * establish these fields' roles; the remaining words are unnamed state. */

void func_0010604c(void)
{
    bss_00495c34.ready = 0;
    bss_00495c34.state04 = 0;
    bss_00495c34.state08 = 0;
    bss_00495c34.state0c = 0;
    bss_00495c34.value = 0;
}
