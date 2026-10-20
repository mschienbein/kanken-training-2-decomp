#include "ctr/cp15.h"
#include "ctr/svc.h"

CtrResult func_00137c20(const CtrHandle *session, CtrHandle handle,
                       const void *buffer, unsigned size)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 0x001f0082u;
    command[1] = (unsigned int)buffer;
    command[2] = size;
    command[3] = 0;
    command[4] = handle;
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    __schedule_barrier();
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
