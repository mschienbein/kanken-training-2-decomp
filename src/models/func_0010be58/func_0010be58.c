#include "game/result.h"
#include "ctr/svc.h"
/* [INFERENCE] A failure-bit accessor may have been armlink-inlined and removed;
 * matching this caller does not recover its historical helper identity. */
extern CtrHandle data_00243128;
extern CtrResult func_001109b4(CtrHandle *handle);
void func_0010be58(void)
{
    if (data_00243128 == 0) {
        CtrHandle handle = 0;
        struct Result result;
        result.value = (unsigned)func_001109b4(&handle);
        if ((int)result_failure_bits(result) >= 0)
            data_00243128 = handle;
    }
}
