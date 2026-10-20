#include "ctr/cp15.h"
#include "ctr/ipc.h"


CtrResult func_00123634(void)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 43u << 16;
    result = svcSendSyncRequest(data_00243040.handle);
    failure = (int)((unsigned int)result & 0x80000000u);
    __schedule_barrier();
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
