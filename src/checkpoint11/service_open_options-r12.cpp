#include "game/service_protocol.h"
#include "game/service_result.h"

/* [INFERENCE] Natural operation-helper identity is unrecovered. */
static ServiceResult open_record(const W23JOpenOptions *options,
    ServiceObject *object, unsigned offset, unsigned size)
{
    Result result = func_00192ae8(object);
    switch (result.failed()) {
    case 0: break;
    default: return ServiceResult(result);
    }
    ServiceResult status(func_0011f6e8(object, offset, size, options));
    func_00192ab0(object, status);
    return status;
}

ServiceResult func_001125ac(ServiceObject *object, unsigned offset,
                            unsigned size, const W23JOpenOptions *options)
{
    return open_record(options, object, offset, size);
}
