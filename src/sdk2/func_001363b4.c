#include "ctr/ipc.h"

union IpcWord {
    unsigned int word;
    unsigned char byte;
};

extern CtrHandle data_0024377c;

unsigned int func_001363b4(unsigned char *out, unsigned char mode,
                         unsigned long long id)
{
    union IpcWord *command = (union IpcWord *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0].word = ctrIpcHeader(4105u, 3u, 0u);
    ctrIpcMarshalByte(&command[1].byte, &mode);
    command[2].word = (unsigned int)id;
    command[3].word = (unsigned int)(id >> 32);
    result = svcSendSyncRequest(data_0024377c);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *out = command[2].byte;
        result = (CtrResult)command[1].word;
    }
    return (unsigned int)result;
}
