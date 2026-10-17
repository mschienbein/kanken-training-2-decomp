#include "ctr/ipc.h"

extern "C" {

CtrResult func_00115a98(unsigned char value)
{
    union ByteCommandWord *command =
        (union ByteCommandWord *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0].word = (34u << 16) | (1u << 6);
    ctrMarshalValue(command[1].byte, value);
    result = svcSendSyncRequest(data_00243040);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0)
        result = (CtrResult)command[1].word;
    return result;
}

}
