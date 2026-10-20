#include "game/service_runtime.h"
#include "game/service_state.h"
#include "ctr/svc.h"
#include "game/applet.h"


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
                if (bss_0049eee4.resumed[index])
                    bss_0049eee4.resumed[index]();
            data_00242afc.pending = 0;
            FinishPublication(data_00242afc.session, data_00242afc.acquired);
        }
    }
}
