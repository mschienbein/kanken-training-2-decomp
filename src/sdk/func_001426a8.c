#include "game/service_protocol.h"
#include "ctr/cp15.h"
#include "ctr/svc.h"

extern CtrHandle data_002430d8;

CtrResult func_001426a8(const void *buffer, unsigned int size)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    unsigned int descriptor = 0;
    const CtrHandle *session;
    int failure;

    command[0] = 0x30042;
    command[1] = size;
    command[3] = (unsigned int)buffer;
    __schedule_barrier();
    session = &data_002430d8;
    size = descriptor | (size << 14);
    __schedule_barrier();
    size |= 2;
    command[2] = size;
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    __schedule_barrier();
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
