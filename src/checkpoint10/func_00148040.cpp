#include "ctr/svc.h"
#include "game/result.h"
struct ResourceServiceSessionState {
    unsigned char reserved00[8];
    const CtrHandle *session;
};
/* INTEGRATOR: differs from include/game/service_runtime.h: +8 is a session
 * pointer passed to func_0014af08, which dereferences it, not a scalar flag. */
/* INTEGRATOR: move to include/game/service_runtime.h */
extern "C" {
extern struct ResourceServiceSessionState data_00242afc;
extern CtrResult func_0014af08(const CtrHandle *session, unsigned int value,
                            unsigned int reserved, void *buffer,
                            unsigned short capacity, unsigned short *output);
}

extern "C" Result func_00148040(unsigned int value, void *buffer,
                              unsigned short capacity, unsigned short *output)
{
    Result result(0xc8a0a7f8u);
    if (data_00242afc.session)
        result = Result((unsigned int)func_0014af08(data_00242afc.session,
                            value, 0, buffer, capacity, output));
    else
        *output = 0;
    return result;
}
