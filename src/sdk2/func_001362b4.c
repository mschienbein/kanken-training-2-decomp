#include "ctr/ipc.h"

extern CtrHandle data_00242ff8;

CtrResult func_001362b4(unsigned int *out)
{
    unsigned int *command = (unsigned int *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0] = ctrIpcHeader(10u, 0u, 0u);
    result = svcSendSyncRequest(data_00242ff8);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *out = command[2];
        result = (CtrResult)command[1];
    }
    return result;
}
