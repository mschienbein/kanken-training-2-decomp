/* [INFERENCE] Signature: Callee packet puts r0 in the IPC buffer-address descriptor, encodes r1 as its size and uses r2 as the request handle; callers consume the one-word result. */


#include "ctr/ipc.h"
unsigned func_00121764(void *source, unsigned size, unsigned handle)
{
    return func_00121768(source, size, handle);
}
