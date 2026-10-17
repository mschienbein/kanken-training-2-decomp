#include "game/service_state.h"
#include "ctr/cp15.h"
#include "ctr/svc.h"
#include <string.h>


CtrResult func_00112494(CtrHandle handle)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    unsigned int handle_descriptor = 0;
    CtrResult result;
    int failure;

    command[0] = (8u << 16) | 4u;
    command[1] = 0x20;
    command[3] = handle_descriptor;
    memcpy(&command[4], &handle, sizeof(handle));
    result = svcSendSyncRequest(data_00242ff8);
    failure = (int)((unsigned int)result & 0x80000000u);
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
