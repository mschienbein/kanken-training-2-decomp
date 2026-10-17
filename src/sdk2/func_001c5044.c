#include "ctr/ipc.h"

CtrResult func_001c5044(const CtrHandle *session, unsigned long long size)
{
    unsigned int *command = (unsigned int *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0] = ctrIpcHeader(2053u, 2u, 0u);
    memcpy(&command[1], &size, sizeof(size));
    result = svcSendSyncRequest(*session);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
