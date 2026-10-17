#include "ctr/ipc.h"

union IpcWord {
    unsigned int word;
    unsigned char byte;
};

extern CtrHandle data_00242ff8;

CtrResult func_0013622c(unsigned int value, unsigned char *out)
{
    union IpcWord *command = (union IpcWord *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0].word = ctrIpcHeader(62u, 1u, 2u);
    command[1].word = value;

    command[2].word = 0x20;
    result = svcSendSyncRequest(data_00242ff8);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *out = command[2].byte;
        result = (CtrResult)command[1].word;
    }
    return result;
}
