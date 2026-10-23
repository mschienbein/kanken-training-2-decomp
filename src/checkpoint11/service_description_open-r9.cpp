#include "game/service_protocol.h"
#include "game/service_result.h"

/* [INFERENCE] Private counted-operation helper; historical helper identity is unrecovered. */
static ServiceResult open_service_records(const ServiceConfigurationData *configuration,
    const ServiceDescription *specification,
    ServiceObject *object, unsigned count)
{
    Result result = func_0018fb88(specification, count);
    switch (result.failed()) {
    case 0: break;
    default: return ServiceResult(result);
    }
    result = func_00192ae8(object);
    switch (result.failed()) {
    case 0: break;
    default: return ServiceResult(result);
    }
    ServiceResult status = func_001209bc(object, specification, configuration, count, 0);
    func_00192ab0(object, status);
    return status;
}

ServiceResult func_00112f10(ServiceObject *object,
    const ServiceDescription *specification,
    const ServiceConfigurationData *configuration)
{
    return open_service_records(configuration, specification, object, 1);
}
