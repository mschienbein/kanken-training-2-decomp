#include <string.h>
#include "ctr/ipc.h"

extern "C" CtrResult func_0012fa6c(const CtrHandle *session, CtrHandle handle, unsigned char mode)
{
    union ByteCommandWord *command = (union ByteCommandWord *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0].word = (0x16u << 16) | (1u << 6) | 2u;
    memcpy(&command[1].byte, &mode, sizeof(mode));
    command[3].word = handle;
    command[2].word = 0;
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0)
        result = (CtrResult)command[1].word;
    return result;
}
