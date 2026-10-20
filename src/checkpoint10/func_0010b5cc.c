#include "game/service_state.h"
#include "game/synchronization.h"

/* Interior binding for AppletStartupState.event.signal at data_0023fdd8 + 8;
 * it denotes the same event storage, not a separate synchronization object. */
extern struct SignalState data_0023fde0;

/* Registered through func_0010ce44 with a null context. The lifecycle registry
 * forwards that context to its hook. [INFERENCE] Opaque pointer type. */
void func_0010b5cc(void *context)
{
    func_0019ea6c(&data_0023fde0);
    data_0023fdd8.requested = 1;
}
