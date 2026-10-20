#include <string.h>
#include "game/result.h"
#include "game/service_clients.h"



extern "C" Result func_00113c0c(unsigned *handle, const char *name)
{
    if (data_002430dc)
        return Result(0xe0a0d3f9u);
    int initialized = func_0019edcc();
    if (initialized < 0)
        return Result((unsigned)initialized);
    unsigned result = func_00192b44(handle, name, strlen(name), 0);
    if (result >> 31)
        return Result(result);
    data_002430dc = 1;
    return Result(0);
}
