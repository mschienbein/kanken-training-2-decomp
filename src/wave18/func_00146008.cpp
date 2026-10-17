#include "game/service_runtime.h"
#include "game/result.h"



Result func_00146008(unsigned short id, unsigned short *value)
{
    Result result = 0xc8a0a7f8u;
    if (data_00242afc.initialized)
        result = func_001481cc(data_00242afc.initialized, id, value);
    return result;
}
