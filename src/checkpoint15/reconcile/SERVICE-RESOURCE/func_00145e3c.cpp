#include "ctr/ipc.h"
#include "game/service_runtime.h"
#include "game/result.h"

#include "game/service_lock.h"

extern "C" CtrResult func_00145e3c(unsigned command, void *buffer, unsigned size)
{
    Result result(0xc8a0a7f8u);
    const CtrHandle *session = data_00242afc.session;
    if (session)
        result.value = func_00148150(session, command, buffer, size);
    return static_cast<CtrResult>(result.value);
}
