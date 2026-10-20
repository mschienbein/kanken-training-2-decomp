#define NW_RELEASE
#include "game/selection_cleanup.h"
#include "game/resource_runtime.h"
#include "game/command_state.h"

static inline void reset_handle(DisplayHandle *handle)
{
    if (handle)
        handle->object = 0;
}

extern "C" void func_001685c0(DisplayHandle **handles)
{
    for (int i = 0; i < 4; ++i) {
        DisplayHandle *handle = handles[i];
        if (handle) {
            if (handle->object)
                func_00183208(handle->object);
            reset_handle(handles[i]);
        }
    }
    func_0017a938(bss_00290d28, -1, 0);
}
