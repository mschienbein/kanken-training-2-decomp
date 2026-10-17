#include "ctr/svc.h"
#include "game/result.h"
struct ResourceServiceSessionState {
    unsigned char reserved00[8];
    const CtrHandle *session;
};
/* INTEGRATOR: differs from include/game/service_runtime.h: +8 is a session
 * pointer passed to func_0014af70, which dereferences it, not a scalar flag. */
/* INTEGRATOR: move to include/game/service_runtime.h */
extern "C" {
extern struct ResourceServiceSessionState data_00242afc;
extern CtrResult func_0014af70(const CtrHandle *session, unsigned int value,
                            unsigned int *output);
}

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
