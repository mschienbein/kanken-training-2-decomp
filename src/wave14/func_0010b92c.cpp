#include "game/service_protocol.h"
#include "game/service_result.h"
#include "game/result.h"



ServiceResult func_0010b92c(ServiceObject *object)
{
    Result code = func_00192ae8(object);
    switch (code.failed()) {
    case 0:
        break;
    default:
        return ServiceResult(code);
    }
    ServiceResult result = func_00112de0(object, 0);
    func_00192ab0(object, result);
    return result;
}
