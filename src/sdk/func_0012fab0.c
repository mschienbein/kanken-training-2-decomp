#include "ctr/cp15.h"
#include "ctr/svc.h"
#include "game/graphics_state.h"

unsigned func_0012fab0(const struct GraphicsMutex *session)
{
    unsigned int *command = (unsigned int *)(ctrGetThreadLocalStorage() + 0x80);
    CtrResult result;
    int failure;

    command[0] = 0x19u << 16;
    result = svcSendSyncRequest(session->handle);
    failure = (int)((unsigned int)result & 0x80000000u);
    __schedule_barrier();
    if (failure >= 0)
        result = (CtrResult)command[1];
    return result;
}
