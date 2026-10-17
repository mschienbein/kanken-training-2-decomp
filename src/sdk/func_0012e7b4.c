#include "ctr/svc.h"
#include "game/runtime.h"

extern const CtrHandle data_0021c5d0;

static __forceinline void CheckCloseResult(CtrResult result)
{
    unsigned int failure = (unsigned int)result >> 31;
    if (failure != 0)
        func_0019f568();
}

CtrResult func_0012e7b4(CtrHandle *handle)
{
    CtrResult result;
    if (*handle != 0) {
        result = svcCloseHandle(*handle);
        CheckCloseResult(result);
        *handle = data_0021c5d0;
    } else {
        result = (CtrResult)0xd8a103f7u;
    }
    return result;
}
