#include "ctr/cp15.h"
#include "ctr/svc.h"

CtrResult func_00113404(const CtrHandle *session, unsigned int value)
{
    volatile unsigned int *command = (volatile unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = (2145u << 16) | (1u << 6) | 2u;
    command[1] = value;
    __schedule_barrier();
    command[2] = 0x20;
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    __schedule_barrier();
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
