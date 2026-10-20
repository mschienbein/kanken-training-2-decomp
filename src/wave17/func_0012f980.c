#include "game/functions.h"
#include "game/graphics_state.h"

#include "ctr/ipc.h"

int func_0012f980(void)
{
    if (!func_001847f8())
        return 0x00202bf8;
    return func_00137c5c(&func_001861d4()->handle);
}
