#include "ctr/cp15.h"
#include "ctr/svc.h"

extern CtrHandle data_00243040;

CtrResult func_00143020(unsigned int value)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 0x180040;
    command[1] = value;
    result = svcSendSyncRequest(data_00243040);
    failure = (int)((unsigned int)result & 0x80000000u);
    __schedule_barrier();
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
