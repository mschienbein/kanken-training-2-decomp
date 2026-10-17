#include "ctr/ipc.h"

struct ServiceByteResponse {
    unsigned int header;
    CtrResult result;
    unsigned char value;
};

CtrResult func_0012e528(const CtrHandle *session, unsigned char *out)
{
    struct ServiceByteResponse *command =
        (struct ServiceByteResponse *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command->header = ctrIpcHeader(2071u, 0u, 0u);
    result = svcSendSyncRequest(*session);
    failure = ctrIpcTransportFailure(result);
    if (failure < 0)
        return result;
    *out = command->value;
    return command->result;
}
