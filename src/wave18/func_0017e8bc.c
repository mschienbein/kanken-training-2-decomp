#include "game/functions.h"
#include "game/service_runtime.h"

void func_0017e8bc(struct ExtendedWorkspace *workspace)
{
    if (workspace->initialized) {
        workspace->initialized = 0;
        workspace->buffer = 0;
        workspace->first = 0;
        workspace->second = 0;
        workspace->third = 0;
        workspace->fourth = 0;
        workspace->second_buffer = 0;
        workspace->second_first = 0;
        workspace->second_second = 0;
        workspace->second_third = 0;
        workspace->second_fourth = 0;
    }
}
