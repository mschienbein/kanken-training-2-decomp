#include "ctr/ipc.h"

CtrResult func_00142604(const CtrHandle *session, CtrHandle handle, void *buffer, unsigned int size)
{
    unsigned int *command = (unsigned int *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0] = ctrIpcHeader(19u, 2u, 2u);
    command[4] = handle;
    command[1] = (unsigned int)buffer;
    command[2] = size;
    command[3] = 0;
    result = svcSendSyncRequest(*session);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
