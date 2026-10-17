#include "game/synchronization.h"

struct StartupSignalState {
    unsigned char reserved00;
    unsigned char signaled;
};
extern struct StartupSignalState data_0023fdd8;
extern struct SignalState data_0023fde0;

/* Registered through func_0010ce44 with a null context. The lifecycle registry
 * forwards that context to its hook. [INFERENCE] Opaque pointer type. */
void func_0010b5cc(void *context)
{
    func_0019ea6c(&data_0023fde0);
    data_0023fdd8.signaled = 1;
}
