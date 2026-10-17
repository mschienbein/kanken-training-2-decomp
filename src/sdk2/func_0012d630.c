#include "ctr/ipc.h"

struct ArchiveScalarCommand {
    unsigned int header;
    union { unsigned char mode; unsigned int result; } status;
    union { unsigned long long id; unsigned int value; } argument;
};

extern CtrHandle data_0024377c;

unsigned int func_0012d630(unsigned int *out, unsigned char mode,
                          unsigned long long id)
{
    struct ArchiveScalarCommand *command =
        (struct ArchiveScalarCommand *)ctrIpcCommandBuffer();
    CtrResult result;
    int failure;

    command->header = ctrIpcHeader(4097u, 3u, 0u);
    ctrIpcMarshalByte(&command->status.mode, &mode);
    command->argument.id = id;
    result = svcSendSyncRequest(data_0024377c);
    failure = ctrIpcTransportFailure(result);
    if (failure < 0)
        return (unsigned int)result;
    *out = command->argument.value;
    return command->status.result;
}
