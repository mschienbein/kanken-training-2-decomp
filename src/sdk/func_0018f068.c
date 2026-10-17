#include "ctr/cp15.h"
#include "ctr/svc.h"

extern CtrHandle data_00243040;

struct IpcByteResponse {
    unsigned int header;
    unsigned int value;
    unsigned char output;
};

CtrResult func_0018f068(unsigned int value, unsigned char *output)
{
    struct IpcByteResponse *command =
        (struct IpcByteResponse *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command->header = (9u << 16) | (1u << 6);
    command->value = value;
    result = svcSendSyncRequest(data_00243040);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure < 0)
        return result;
    *output = command->output;
    return (CtrResult)command->value;
}
