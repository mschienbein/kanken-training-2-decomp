#include "ctr/ipc.h"

CtrResult func_00148150(const CtrHandle *session, unsigned int value,
                        void *buffer, unsigned int size)
{
    unsigned int *command = (unsigned int *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command[0] = ctrIpcHeader(13u, 2u, 2u);
    command[1] = value;
    command[2] = size;
    command[3] = ctrIpcStaticBufferDescriptor(size, 1);
    command[4] = (unsigned int)buffer;
    result = svcSendSyncRequest(*session);
    failure = ctrIpcTransportFailure(result);

    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
