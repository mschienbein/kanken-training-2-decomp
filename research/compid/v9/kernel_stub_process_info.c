/* v9 no-frame probe: raw GetProcessInfo service with its 64-bit pointer output. */
#include "ctr/svc.h"

CtrResult func_0011ef2c(long long *value, CtrHandle process, CtrProcessInfoType type)
{
    CtrSvcProcessInfoResult output = svcGetProcessInfo(0, process, type);
    *value = (long long)(((unsigned long long)output.value_high << 32) | output.value_low);
    return output.result;
}
