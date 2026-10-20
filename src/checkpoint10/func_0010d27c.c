#include "game/functions.h"
#include "game/runtime.h"
#include "ctr/svc.h"

extern int func_00195e84(void);
/* [INFERENCE] Buffer/size are forwarded to an IPC static-buffer descriptor;
 * the third input is marshalled in the handle-transfer descriptor. */
extern void func_0010d2b8(const void *buffer, unsigned int size, CtrHandle handle);

void func_0010d27c(const void *buffer, unsigned int size, CtrHandle handle)
{
    if (!func_00195e94() && !func_00195e84())
        func_0019f568();
    func_0010d2b8(buffer, size, handle);
}
