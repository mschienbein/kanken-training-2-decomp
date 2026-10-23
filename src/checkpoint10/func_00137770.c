#include "game/graphics_state.h"
#include "ctr/ipc.h"

extern CtrHandle data_0021c608;

unsigned func_00137770(void *buffer, unsigned size)
{
    void *input = buffer;
    unsigned handle = data_0021c608;
    return func_0013ed08(&func_001861d4()->handle, handle, input, size);
}
