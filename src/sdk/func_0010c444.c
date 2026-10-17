#include "ctr/cp15.h"
#include "ctr/svc.h"

extern CtrHandle data_00243144;

CtrResult func_0010c444(void)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 0x10002;
    __schedule_barrier();
    command[1] = 0x20;
    result = svcSendSyncRequest(data_00243144);
    failure = (int)((unsigned int)result & 0x80000000u);
    __schedule_barrier();
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
