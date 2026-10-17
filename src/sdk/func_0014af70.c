#include "ctr/cp15.h"
#include "ctr/svc.h"

CtrResult func_0014af70(const CtrHandle *session, unsigned int value,
                        unsigned int *output)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = (12u << 16) | (1u << 6);
    command[1] = value;
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure < 0)
        return result;
    *output = command[2];
    return (CtrResult)command[1];
}
