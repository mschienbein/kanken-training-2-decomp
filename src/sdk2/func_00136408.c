#include "ctr/ipc.h"

union IpcWord {
    unsigned int word;
    unsigned char byte;
};

extern CtrHandle data_0024377c;

unsigned int func_00136408(unsigned char *out)
{
    union IpcWord *command = (union IpcWord *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0].word = ctrIpcHeader(4106u, 0u, 0u);
    result = svcSendSyncRequest(data_0024377c);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *out = command[2].byte;
        result = (CtrResult)command[1].word;
    }
    return (unsigned int)result;
}
