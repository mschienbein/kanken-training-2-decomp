#include "ctr/ipc.h"

struct X03IpcByteCommand {
    unsigned header;
    union {
        unsigned result;
        unsigned char value;
    } body;
};

unsigned func_00122a30(const CtrHandle *session, unsigned char value)
{
    struct X03IpcByteCommand *command =
        (struct X03IpcByteCommand *)ctrIpcCommandBuffer();
    unsigned result;
    int failure;

    command->header = ctrIpcHeader(11u, 1u, 0u);
    ctrIpcMarshalByte(&command->body.value, &value);
    result = (unsigned)svcSendSyncRequest(*session);
    failure = (int)(result & 0x80000000u);
    if (failure >= 0)
        result = command->body.result;
    return result;
}
