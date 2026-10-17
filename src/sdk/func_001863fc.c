#include "ctr/cp15.h"
#include "ctr/svc.h"

CtrResult func_001863fc(const CtrHandle *session)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 18u << 16;
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
