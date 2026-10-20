#include "ctr/cp15.h"
#include "ctr/ipc.h"


CtrResult func_0013823c(unsigned int value)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 0x00160040u;
    command[1] = value;
    result = svcSendSyncRequest(data_00243040.handle);
    failure = (int)((unsigned int)result & 0x80000000u);
    __schedule_barrier();
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
