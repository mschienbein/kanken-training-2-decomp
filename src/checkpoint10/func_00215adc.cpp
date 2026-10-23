#include <new>
#include "game/service_state.h"

/* Recovered relative init-array target. Initializes the shared service handle
 * used by func_00112494 and the related service IPC wrappers. The original
 * C++ constructor/type spelling is unrecovered. */
extern "C" void func_00215adc(void)
{
    new (&data_00242ff8) CtrHandle(0);
}
