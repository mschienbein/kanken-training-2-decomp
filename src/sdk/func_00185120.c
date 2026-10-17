#include "ctr/cp15.h"
#include "ctr/svc.h"

CtrResult func_00185120(const CtrHandle *session, unsigned long long identifier)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = (0x80eu << 16) | (2u << 6);
    command[1] = (unsigned int)identifier;
    command[2] = (unsigned int)(identifier >> 32);
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
