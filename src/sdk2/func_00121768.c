#include "ctr/ipc.h"

/* The mapped-buffer permission permits service writes, so source is writable. */
extern CtrHandle data_00243054;

unsigned func_00121768(void *source, unsigned size, unsigned handle)
{
    CtrIpcWord *command = (CtrIpcWord *)ctrIpcCommandBuffer();
    CtrResult result;

    command[0].word = ctrIpcHeader(1u, 2u, 2u);
    command[1].word = size;
    command[2].word = handle;
    command[3].word = ctrIpcMappedBufferDescriptor(size, 4u);
    command[4].word = (unsigned int)source;
    result = svcSendSyncRequest(data_00243054);
    if (ctrIpcTransportFailure(result) < 0)
        return result;
    return command[1].result;
}
