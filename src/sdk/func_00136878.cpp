#include "ctr/ipc.h"

extern "C" CtrResult func_00136878(const CtrHandle *session, unsigned char mode)
{
    union ByteCommandWord *command = (union ByteCommandWord *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0].word = (0x20u << 16) | (1u << 6);
    ctrMarshalValue(command[1].byte, mode);
    result = svcSendSyncRequest(*session);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0)
        result = (CtrResult)command[1].word;
    return result;
}
