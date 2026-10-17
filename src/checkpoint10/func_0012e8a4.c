#include "game/service_runtime.h"
#include "game/service_state.h"
#include "ctr/svc.h"
#include "game/applet.h"

struct ServicePublicationState {
    unsigned char deferred, published, pending;
    signed char acquired;
    unsigned short first, second;
    const CtrHandle *session;
    unsigned busy;
    unsigned char lock_storage[8];
    CtrHandle handle;
    const void *source;
    unsigned size;
};
/* INTEGRATOR: differs from include/game/service_runtime.h: publication uses
 * +4/+6 halfword parameters, a session pointer at +8, and payload at +28/+32. */


extern CtrResult func_00185050(const void *source, unsigned size,
    unsigned short first, unsigned short second);
/* INTEGRATOR: differs from src/sdk/func_00136878.cpp: this caller sign-extends
 * the byte argument. The callee consumes the low byte; reconcile its contract. */
extern CtrResult func_00136878(const CtrHandle *session, signed char mode);

static __forceinline void CheckResult(CtrResult result, const void *location)
{
    unsigned int failure = (unsigned int)result >> 31;
    if (failure != 0)
        func_00192afc(result, location);
}

static __forceinline void FinishPublication(const CtrHandle *session, signed char acquired)
{
    if (session)
        func_00136878(session, acquired);
}

/* The sole direct caller, func_00122b30, ignores the result. [INFERENCE] void
 * follows that caller contract; the tail IPC's result is not exposed here. */
void func_0012e8a4(void)
{
    int index;
    if (data_00242afc.deferred) {
        data_00242afc.deferred = 0;
        if (data_00242afc.pending) {
            CtrResult result = func_00185050(data_00242afc.source, data_00242afc.size,
                data_00242afc.first, data_00242afc.second);
            CheckResult(result, (const void *)__current_pc());
            for (index = 0; index < 8; ++index)
                if (bss_0049eee4[index])
                    bss_0049eee4[index]();
            data_00242afc.pending = 0;
            FinishPublication(data_00242afc.session, data_00242afc.acquired);
        }
    }
}
