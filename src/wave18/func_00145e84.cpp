#include "game/service_runtime.h"
#include <stdint.h>
#include "game/result.h"



Result func_00145e84(uint16_t flags)
{
    Result result(0xc8a0a7f8u);
    const struct ServiceHandle *session = data_00242afc.initialized;
    if (session)
        result = func_00148118(session, flags);
    return result;
}
