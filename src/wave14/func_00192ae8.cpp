#include "game/service_protocol.h"
#include "game/service_result.h"
#include "game/result.h"


Result func_00192ae8(const ServiceObject *object)
{
    Result result(func_0019058c(object));
    switch (result.failed()) {
    case 0:
        return Result(0);
    default:
        return result;
    }
}
