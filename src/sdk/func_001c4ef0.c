#include "ctr/cp15.h"
#include "ctr/svc.h"

CtrResult func_001c4ef0(const CtrHandle *session, CtrHandle *output)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 0x080c0000;
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0) {
        *output = command[3];
        result = (CtrResult)command[1];
    }
    return result;
}
