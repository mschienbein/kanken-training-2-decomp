#include "game/service_callbacks.h"
#include "game/service_state.h"
/* [INFERENCE] The callback accepts its stored context and returns a polling
 * decision. The default callback func_00122b30 ignores the context. */
void func_001233c0(ServicePollCallback callback, void *context)
{
    data_00243008.callback = callback;
    data_00243008.context = context;
}
