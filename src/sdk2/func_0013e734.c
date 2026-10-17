#include "ctr/ipc.h"

struct ByteResponseCommand {
    unsigned int header;
    CtrResult result;
    unsigned char value;
};

extern CtrHandle data_00243054;

CtrResult func_0013e734(unsigned char *output)
{
    struct ByteResponseCommand *command =
        (struct ByteResponseCommand *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command->header = ctrIpcHeader(2u, 0u, 0u);
    result = svcSendSyncRequest(data_00243054);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0) {
        *output = command->value;
        result = command->result;
    }
    return result;
}
