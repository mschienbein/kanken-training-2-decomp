#include "ctr/ipc.h"

struct ByteResponseCommand {
    unsigned int header;
    CtrResult result;
    unsigned char value;
};

extern CtrHandle data_0024376c;

CtrResult func_00138370(unsigned char *output)
{
    struct ByteResponseCommand *command =
        (struct ByteResponseCommand *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command->header = ctrIpcHeader(59u, 0u, 0u);
    result = svcSendSyncRequest(data_0024376c);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *output = command->value;
        result = command->result;
    }
    return result;
}
