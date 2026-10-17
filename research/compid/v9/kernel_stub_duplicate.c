/* v9 no-frame probe: raw DuplicateHandle service with its pointer output. */
#include "ctr/svc.h"

CtrResult func_001565b0(CtrHandle *out, CtrHandle original)
{
    CtrSvcHandleResult result = svcDuplicateHandle(0, original);
    *out = result.handle;
    return result.result;
}
