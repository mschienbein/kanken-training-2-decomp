#include "ctr/cp15.h"
#include "ctr/svc.h"

extern CtrHandle data_00242ff8;

CtrResult func_001124dc(void)
{
    volatile unsigned int *command = (volatile unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = (9u << 16) | 2u;
    __schedule_barrier();
    command[1] = 0x20;
    result = svcSendSyncRequest(data_00242ff8);
    failure = (int)((unsigned int)result & 0x80000000u);
    __schedule_barrier();
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
