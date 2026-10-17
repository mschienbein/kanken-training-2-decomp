#include "game/graphics_state.h"
#include "ctr/svc.h"

extern CtrHandle data_0021c608;
/* INTEGRATOR: differs from src/sdk/func_00137c20.c: callers of func_0018f6c4
 * pass a buffer pointer and byte length (including fixed 2496-byte blocks),
 * not a single 64-bit token. The IPC normal words are pointer and size. */
extern CtrResult func_00137c20(const CtrHandle *session, CtrHandle handle,
                              const void *buffer, unsigned size);

int func_0012f570(const void *buffer, unsigned size)
{
    const void *input = buffer;
    unsigned handle = data_0021c608;
    return func_00137c20(&func_001861d4()->handle, handle, input, size);
}
