#include "ctr/ipc.h"

struct IpcByteCommand {
    unsigned int header;
    union {
        unsigned int word;
        unsigned char byte;
    } value;
};

extern CtrHandle data_0024376c;

CtrResult func_00138334(unsigned char value)
{
    struct IpcByteCommand *command =
        (struct IpcByteCommand *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command->header = ctrIpcHeader(61u, 1u, 0u);
    ctrIpcMarshalByte(&command->value.byte, &value);
    result = svcSendSyncRequest(data_0024376c);
    failure = ctrIpcTransportFailure(result);

    if (failure >= 0)
        result = (CtrResult)command->value.word;
    return result;
}
