#include "ctr/ipc.h"


unsigned func_00137070(unsigned *out)
{
    unsigned int *command =
        (unsigned int *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0] = ctrIpcHeader(29u, 0u, 0u);
    result = svcSendSyncRequest(data_002430d8.handle);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *out = command[2];
        result = (CtrResult)command[1];
    }
    return (unsigned)result;
}
