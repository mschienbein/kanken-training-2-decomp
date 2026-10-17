#include "ctr/ipc.h"

struct FileSizeReply {
    unsigned int header;
    CtrResult result;
    unsigned long long size;
};

CtrResult func_001c500c(const CtrHandle *session, unsigned long long *size)
{
    struct FileSizeReply *command =
        (struct FileSizeReply *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command->header = ctrIpcHeader(2052u, 0u, 0u);
    result = svcSendSyncRequest(*session);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        memcpy(size, &command->size, sizeof(*size));
        result = command->result;
    }
    return result;
}
