#include "game/service_runtime.h"
#include "game/result.h"



Result func_00145e08(unsigned short id, unsigned char *value)
{
    Result result = 0xc8a0a7f8u;
    if (data_00242afc.initialized)
        result = func_001480d4(data_00242afc.initialized, id, value);
    return result;
}
