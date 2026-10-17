#include "ctr/cp15.h"
#include "ctr/svc.h"

extern CtrHandle data_00243040;

CtrResult func_0012feb0(unsigned int selector, unsigned int limit)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = (0x42u << 16) | (2u << 6);
    command[1] = selector;
    command[2] = limit;
    result = svcSendSyncRequest(data_00243040);
    failure = (int)((unsigned int)result & 0x80000000u);
    __schedule_barrier();
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
