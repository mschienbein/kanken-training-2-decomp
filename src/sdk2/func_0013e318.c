#include "ctr/ipc.h"

extern CtrHandle data_00242ff8;

CtrResult func_0013e318(void *input, unsigned char *output)
{
    CtrIpcWord *command = (CtrIpcWord *)ctrIpcCommandBuffer();
    CtrResult result;

    command[0].word = ctrIpcHeader(39u, 0u, 2u);
    command[1].word = 0x800402;
    command[2].word = (unsigned int)input;
    result = svcSendSyncRequest(data_00242ff8);
    if (ctrIpcTransportFailure(result) < 0)
        return result;
    *output = command[2].byte;
    return command[1].result;
}
