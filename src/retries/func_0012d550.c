#include "ctr/ipc.h"
#include "game/archive_ipc.h"

CtrResult func_0012d550(unsigned int *out, struct ArchiveInfo *buffer,
                        unsigned int count, unsigned char mode,
                        unsigned long long id, unsigned int flags)
{
    struct ArchiveOutputCommand *command =
        (struct ArchiveOutputCommand *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;

    command->header = (4099u << 16) | (5u << 6) | 2u;
    command->count = count;
    ctrIpcMarshalByte(&command->selector.mode, &mode);
    command->id_low = (unsigned int)id;
    command->id_high = (unsigned int)(id >> 32);
    command->flags = flags;
    command->descriptor = ((count * 3u) << 7) | 12u;
    command->buffer = buffer;
    result = svcSendSyncRequest(data_0024377c);
    if (ctrIpcTransportFailure(result) < 0)
        return result;
    *out = command->selector.value;
    return (CtrResult)command->count;
}
