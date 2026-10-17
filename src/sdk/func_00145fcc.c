#include "ctr/cp15.h"
#include "ctr/svc.h"

CtrResult func_00145fcc(const CtrHandle *session, CtrHandle handle,
                        unsigned long long value)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 0x150082;
    command[4] = handle;
    command[1] = (unsigned int)value;
    command[2] = (unsigned int)(value >> 32);
    command[3] = 0;
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    __schedule_barrier();
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
