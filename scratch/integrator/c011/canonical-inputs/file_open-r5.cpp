#include "game/file_open.h"

/* [INFERENCE] Source-level operation helper; the historical name and opaque
 * argument roles are unrecovered. No additional external callee is introduced. */
static ServiceResult open_service_record(const FileOpenOptions *options,
    ServiceObject *object, unsigned offset, unsigned size)
{
    Result result = func_00192ae8(object);
    switch (result.failed()) {
    case 0: break;
    default: return ServiceResult(result);
    }
    result = func_0011fca0(object, offset, size, options);
    ServiceResult status(result);
    func_00192ab0(object, status);
    return status;
}

ServiceResult func_00112ae0(ServiceObject *object, unsigned offset,
                          unsigned size, const FileOpenOptions *options)
{
    return open_service_record(options, object, offset, size);
}
