#include "game/graphics_state.h"
#include "ctr/svc.h"
#include "ctr/ipc.h"


int func_0012f570(const void *buffer, unsigned size)
{
    const void *input = buffer;
    unsigned handle = data_0021c608;
    return func_00137c20(&func_001861d4()->handle, handle, input, size);
}
