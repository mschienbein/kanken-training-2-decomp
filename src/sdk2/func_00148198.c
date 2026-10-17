#include "ctr/ipc.h"

CtrResult func_00148198(const CtrHandle *session, CtrHandle *output)
{
    unsigned int *command = (unsigned int *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0] = ctrIpcHeader(22u, 0u, 0u);
    result = svcSendSyncRequest(*session);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *output = command[3];
        result = (CtrResult)command[1];
    }
    return result;
}
