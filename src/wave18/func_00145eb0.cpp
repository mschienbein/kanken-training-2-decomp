#include "game/service_runtime.h"
#include "game/result.h"



Result func_00145eb0(unsigned int *output)
{
    Result result(0xc8a0a7f8u);
    const struct ServiceHandle *session = data_00242afc.initialized;
    if (session)
        result = func_00148198(&session->handle, output);
    return result;
}
