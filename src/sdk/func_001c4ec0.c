#include "ctr/cp15.h"
#include "ctr/svc.h"

CtrResult func_001c4ec0(const CtrHandle *session, unsigned int priority)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 0x080a0040;
    command[1] = priority;
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
