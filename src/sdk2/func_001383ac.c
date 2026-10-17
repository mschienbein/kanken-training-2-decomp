#include "ctr/ipc.h"

struct ByteInputCommand {
    unsigned int header;
    union {
        CtrResult result;
        unsigned char value;
    } body;
};
extern CtrHandle data_0024376c;

CtrResult func_001383ac(unsigned char value)
{
    struct ByteInputCommand *command =
        (struct ByteInputCommand *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command->header = ctrIpcHeader(19u, 1u, 0u);
    ctrIpcMarshalByte(&command->body.value, &value);
    result = svcSendSyncRequest(data_0024376c);
    failure = ctrIpcTransportFailure(result);
    if (failure >= 0)
        result = command->body.result;
    return result;
}
