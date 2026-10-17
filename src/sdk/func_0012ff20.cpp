#include "ctr/ipc.h"

extern "C" CtrResult func_0012ff20(unsigned int value, unsigned char enabled)
{
    union ByteCommandWord *command = (union ByteCommandWord *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0].word = (0x3eu << 16) | (2u << 6);
    command[1].word = value;
    ctrMarshalValue(command[2].byte, enabled);
    result = svcSendSyncRequest(data_00243040);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0)
        result = (CtrResult)command[1].word;
    return result;
}
