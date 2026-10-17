#include "game/result.h"
#include <new>

/* [INFERENCE] Only the service-runtime result slot at +0x24 is recovered.
 * INTEGRATOR: move ServiceStartupState to include/game/service_runtime.h. */
struct ServiceStartupState {
    unsigned char reserved00[0x24];
    struct Result result;
};

/* INTEGRATOR: move to include/game/service_runtime.h. */
extern "C" {
extern struct ServiceStartupState bss_0049f000;
void func_0020e32c(void);
}

/* Recovered startup function constructing the initial error result. */
void func_0020e32c(void)
{
    new (&bss_0049f000.result) Result(0xe7e3ffffu);
}
