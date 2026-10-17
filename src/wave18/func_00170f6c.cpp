#include "game/screen.h"
#include "game/display_groups.h"



extern "C" void func_00170f6c(W18DisplayCollection *collection)
{
    for (int i = 0; i < 10; ++i) {
        DisplayHandle *handle = collection->handles[i];
        if (handle != 0) {
            if (handle->object != 0)
                func_00183208(handle->object);
            handle = collection->handles[i];
            if (handle != 0)
                handle->object = 0;
        }
    }
    func_0017a938(&bss_00290e98, -1, 0);
}
