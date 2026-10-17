#include "game/service_state.h"
#include "ctr/cp15.h"
#include "ctr/svc.h"


CtrResult func_0012349c(unsigned *first, unsigned *second)
{
    unsigned *output = first;
    unsigned *command = (unsigned *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 0x00440000u;
    result = svcSendSyncRequest(data_00243040);
    failure = (int)((unsigned)result & 0x80000000u);
    if (failure >= 0) {
        *second = command[2];
        *output = command[4];
        result = (CtrResult)command[1];
    }
    return result;
}
