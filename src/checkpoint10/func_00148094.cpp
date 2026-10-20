#include "game/service_runtime.h"
#include "ctr/svc.h"
#include "game/result.h"

extern "C" Result func_00148094(unsigned int value, unsigned int *output)
{
    Result result(0xc8a0a7f8u);
    *output = ~0u;
    if (data_00242afc.session) {
        /* ORIGINAL BUG: the service result is discarded; failure is returned
         * even when the service has successfully populated the output. */
        func_0014af70(data_00242afc.session, value, output);
    }
    return result;
}
