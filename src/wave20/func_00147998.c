#include "game/functions.h"
#include "game/task_runtime.h"
#include "game/synchronization.h"

void func_00147998(unsigned value)
{
    if (bss_0029f540.active != 0 && value != (unsigned)-1 &&
        bss_0029f540.ready != 0) {
        bss_0029f540.values[bss_0029f540.write_index] = value;
        ++bss_0029f540.write_index;
        if (bss_0029f540.write_index >= 48)
            bss_0029f540.write_index = 0;
        bss_0029f540.pending = 1;
        func_0019ea6c(&bss_0029f540.signal);
    }
}
