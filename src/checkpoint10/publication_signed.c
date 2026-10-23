#include "game/service_runtime.h"
#include "game/service_state.h"
#include "game/applet.h"
#include "ctr/svc.h"

/* [INFERENCE] Dispatch sets up no explicit callback arguments; callback
 * identities and their complete contracts remain to be adjudicated. */

static __inline void CheckResult(const void *location, CtrResult result)
{
    switch ((unsigned)result >> 31) {
    case 0: break;
    default: func_00192afc(result, location); break;
    }
}
static CtrResult release_optional_session(const CtrHandle *session,
                                          signed char mode)
{
    if (session)
        return func_00136878(session, mode);
    return 0;
}
CtrResult func_001368b0(void)
{
    CtrResult result;
    int index;
    data_00242afc.deferred = 0;
    if (!data_00242afc.pending)
        return 0;
    result = func_00185050(data_00242afc.source, data_00242afc.size,
        data_00242afc.first, data_00242afc.second);
    CheckResult((const void *)__current_pc(), result);
    for (index = 0; index < 8; ++index)
        if (bss_0049eee4.resumed[index])
            bss_0049eee4.resumed[index]();
    data_00242afc.pending = 0;
    return release_optional_session(data_00242afc.session, data_00242afc.acquired);
}
