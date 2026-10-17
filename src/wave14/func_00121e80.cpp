#include "game/service_availability.h"
#include "game/service_protocol.h"
#include "game/service_result.h"
#include "game/result.h"


Result func_00121e80(ServiceConfigurationWords *information)
{
    if (!func_00186538())
        return Result(0xc8a0d018u);
    return func_0012f170(information);
}
