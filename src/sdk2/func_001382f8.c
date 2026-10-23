#include "ctr/ipc.h"

struct IpcByteResponse {
    unsigned int header;
    CtrResult result;
    unsigned char value;
};



CtrResult func_001382f8(unsigned char *out)
{
    struct IpcByteResponse *command =
        (struct IpcByteResponse *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command->header = ctrIpcHeader(60u, 0u, 0u);
    result = svcSendSyncRequest(data_0024376c.handle);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *out = command->value;
        result = command->result;
    }
    return result;
}
