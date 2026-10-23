#include "ctr/ipc.h"
#include "game/applet.h"

extern const CtrHandle data_0021c5cc;

static __forceinline CtrResult CloseSession(CtrHandle *session)
{
    CtrResult result;
    if (*session == 0)
        return (CtrResult)0xe0a0cff8u;
    result = svcCloseHandle(*session);
    *session = data_0021c5cc;
    return result;
}

static __forceinline void CheckResult(CtrResult result, const void *location)
{
    unsigned int failure = (unsigned int)result >> 31;
    if (failure != 0)
        func_00192afc(result, location);
}

CtrResult func_0011516c(void)
{
    CtrResult result = CloseSession(&data_00243040.handle);
    CheckResult(result, (const void *)__current_pc());
    return result;
}
