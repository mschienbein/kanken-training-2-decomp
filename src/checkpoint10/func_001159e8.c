/* [INFERENCE] Register storage hints concern real output pointers; original spelling is unrecovered. */
#include "ctr/ipc.h"

CtrResult func_001159e8(register CtrHandle *handle, unsigned int flags,
                        unsigned int *attributes, unsigned int *state)
{
    unsigned int *command = (unsigned int *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0] = ctrIpcHeader(1u, 1u, 0u);
    command[1] = flags;
    result = svcSendSyncRequest(data_00243040);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *attributes = command[2];
        *state = command[3];
        *handle = command[5];
        result = (CtrResult)command[1];
    }
    return result;
}
