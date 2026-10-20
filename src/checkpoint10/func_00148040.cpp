#include "game/service_runtime.h"
#include "ctr/svc.h"
#include "game/result.h"

extern "C" Result func_00148040(unsigned int value, void *buffer,
                              unsigned short capacity, unsigned short *output)
{
    Result result(0xc8a0a7f8u);
    if (data_00242afc.session)
        result = Result((unsigned int)func_0014af08(data_00242afc.session,
                            value, 0, buffer, capacity, output));
    else
        *output = 0;
    return result;
}
