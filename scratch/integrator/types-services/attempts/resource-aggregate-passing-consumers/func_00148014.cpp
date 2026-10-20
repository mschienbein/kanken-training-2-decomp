#include "game/service_runtime.h"
#include <stdint.h>
#include "game/result.h"



Result func_00148014(uint16_t flags)
{
    Result result(0xc8a0a7f8u);
    const CtrHandle *session = data_00242afc.session;
    if (session)
        result = func_0014aed0(session, flags);
    return result;
}
