#include "ctr/ipc.h"

extern "C" {

CtrResult func_001235f8(unsigned char selector)
{
    struct ByteSelectorCommand *command =
        (struct ByteSelectorCommand *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command->header = (59u << 16) | (1u << 6);
    ctrMarshalValue(command->status.selector, selector);
    result = svcSendSyncRequest(data_00243040);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0)
        result = command->status.result;
    return result;
}

}
