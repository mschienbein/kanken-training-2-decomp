#include "ctr/cp15.h"
#include "ctr/svc.h"

CtrResult func_0012fab0(const CtrHandle *session)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 0x19u << 16;
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    __schedule_barrier();
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
