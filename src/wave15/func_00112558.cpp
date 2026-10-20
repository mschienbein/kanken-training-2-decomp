#include <string.h>
#include "game/result.h"
#include "game/service_clients.h"



extern "C" Result func_00112558(void)
{
    if (!data_0024377c) {
        Result result = func_0019edcc();
        switch (result.failed()) {
        case 0: break;
        default: return result;
        }
        result = func_00192b44(&data_0024377c, data_0021c5d9,
                              strlen(data_0021c5d9), 0);
        switch (result.failed()) {
        case 0: break;
        default: return result;
        }
    }
    return Result(0);
}
