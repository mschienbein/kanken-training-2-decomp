#include "game/service_callbacks.h"
#include "game/service_state.h"
/* [INFERENCE] Callback roles are provisional; recovered parameter shapes follow
 * func_00122b30's indirect calls. Slots 11 and 15 have no recovered invocation,
 * so their signatures use the void(void) fallback. Slots 13 and 14 are unchanged. */
void func_00115084(void)
{
    func_001233c0(func_00122b30, 0);
    data_00242a6c.callbacks.request = 0;
    data_00242a6c.contexts[0] = 0;
    data_00242a6c.callbacks.message = 0;
    data_00242a6c.contexts[1] = 0;
    data_00242a6c.callbacks.receive = 0;
    data_00242a6c.contexts[2] = 0;
    data_00242a6c.callbacks.begin = 0;
    data_00242a6c.contexts[3] = 0;
    data_00242a6c.callbacks.end = 0;
    data_00242a6c.contexts[4] = 0;
    data_00242a6c.callbacks.poll = 0;
    data_00242a6c.contexts[5] = 0;
    data_00242a6c.callbacks.suspend = 0;
    data_00242a6c.contexts[6] = 0;
    data_00242a6c.callbacks.resume = 0;
    data_00242a6c.contexts[7] = 0;
    data_00242a6c.callbacks.stop = 0;
    data_00242a6c.contexts[8] = 0;
    data_00242a6c.callbacks.pause = 0;
    data_00242a6c.contexts[9] = 0;
    data_00242a6c.callbacks.complete = 0;
    data_00242a6c.contexts[10] = 0;
    data_00242a6c.callbacks.reserved_callback = 0;
    data_00242a6c.contexts[11] = 0;
    data_00242a6c.callbacks.ready = 0;
    data_00242a6c.contexts[12] = 0;
    data_00242a6c.callbacks.shutdown = 0;
    data_00242a6c.contexts[15] = 0;
}
