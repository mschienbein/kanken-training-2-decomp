#include <string.h>
#include "game/result.h"
#include "game/service_clients.h"

extern "C" Result func_00113c0c(unsigned *handle, const char *name)
{
    if (data_002430dc)
        return Result(0xe0a0d3f9u);
    Result result = func_0019edcc();
    switch (result.failed()) {
    case 0: break;
    default: return result;
    }
    result = func_00192b44(handle, name, strlen(name), 0);
    switch (result.failed()) {
    case 0: break;
    default: return result;
    }
    data_002430dc = 1;
    return Result(0);
}
