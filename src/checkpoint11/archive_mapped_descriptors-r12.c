#include "ctr/ipc.h"

#include "game/archive_ipc.h"
typedef unsigned long long EncodedId;

/* [INFERENCE] Encodes a mapped-buffer byte count and its permission tag. */
static unsigned mapped_descriptor(unsigned bytes, unsigned descriptor_flags)
{
    return (bytes << 4) | descriptor_flags;
}

unsigned int func_0012d5c4(struct ArchiveInfo *out, unsigned char mode,
                          const EncodedId *ids, unsigned int count)
{
    CtrIpcWord *command = (CtrIpcWord *)ctrIpcCommandBuffer();
    CtrResult result;

    command[0].word = ctrIpcHeader(4101u, 2u, 4u);
    ctrIpcMarshalByte(&command[1].byte, &mode);
    command[2].word = count;
    command[3].word = mapped_descriptor(count * sizeof(*ids), 10u);
    command[4].word = (unsigned int)ids;
    command[5].word = mapped_descriptor(count * 24u, 12u);
    command[6].word = (unsigned int)out;
    result = svcSendSyncRequest(data_0024377c);
    if (ctrIpcTransportFailure(result) < 0)
        return result;
    return command[1].result;
}
