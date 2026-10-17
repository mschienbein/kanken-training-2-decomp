#include "ctr/ipc.h"

extern CtrHandle data_00243040;

struct IpcNormalWords_00115a44 {
    unsigned int size;
    unsigned int reserved;
};

struct IpcRequest_00115a44 {
    unsigned int header;
    struct IpcNormalWords_00115a44 parameters;
    CtrHandle handle;
    unsigned int descriptor;
    const void *buffer;
};

CtrResult func_00115a44(const void *buffer, unsigned int size, CtrHandle handle)
{
    struct IpcRequest_00115a44 *command =
        (struct IpcRequest_00115a44 *)ctrIpcCommandBuffer();
    struct IpcNormalWords_00115a44 parameters;
    unsigned int descriptor = 0;
    CtrResult result;
    int failure;

    parameters.size = size;
    parameters.reserved = descriptor;
    command->header = ctrIpcHeader(39u, 1u, 4u);
    command->handle = handle;
    command->parameters = parameters;

    descriptor |= size << 14;
    descriptor |= 2u;
    command->buffer = buffer;
    command->descriptor = descriptor;
    result = svcSendSyncRequest(data_00243040);
    failure = ctrIpcTransportFailure(result);

    if (failure >= 0)
        result = (CtrResult)command->parameters.size;
    return result;
}
