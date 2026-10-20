#include "ctr/ipc.h"

union IpcWord {
    unsigned int word;
    unsigned char byte;
};



CtrResult func_0012ff64(unsigned int value, unsigned char *out)
{
    union IpcWord *command = (union IpcWord *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0].word = ctrIpcHeader(11u, 1u, 0u);
    command[1].word = value;
    result = svcSendSyncRequest(data_00243040.handle);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *out = command[2].byte;
        result = (CtrResult)command[1].word;
    }
    return result;
}
