#include "ctr/ipc.h"

extern CtrHandle data_00243144;

CtrResult func_00113da0(CtrHandle *handle)
{
    unsigned int *command = (unsigned int *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0] = ctrIpcHeader(2u, 0u, 0u);
    result = svcSendSyncRequest(data_00243144);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *handle = command[3];
        result = (CtrResult)command[1];
    }
    return result;
}
