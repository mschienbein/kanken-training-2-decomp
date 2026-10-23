#include <string.h>
#include "game/service_result.h"
#include "game/service_protocol.h"
#include "game/service_availability.h"
#include "game/service_configuration_words.h"

static void copy_configuration_prefix(void *destination,
                                       const ServiceConfigurationWords *source,
                                       unsigned size)
{
    memcpy(destination, source, size);
}


static void query_configuration_prefix(void *information, Result *code)
{
    ServiceConfigurationWords all;
    *code = func_00121e80(&all);
    switch (code->failed()) {
    case 0:
        copy_configuration_prefix(information, &all, 32);
        *code = Result(0);
        break;
    default:
        break;
    }
}

ServiceResult func_00112bf8(ServiceObject *object, void *information)
{
    Result code = func_00192ae8(object);
    switch (code.failed()) {
    case 0: break;
    default: return ServiceResult(code);
    }
    query_configuration_prefix(information, &code);
    ServiceResult result(code);
    func_00192ab0(object, result);
    return result;
}
