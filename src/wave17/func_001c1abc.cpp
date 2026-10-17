#include "game/screen.h"
#include "game/display_groups.h"



extern "C" void func_001c1abc(W17SmallCollection *collection)
{
    for (int i = 0; i < 9; ++i) {
        DisplayHandle *handle = collection->handles[i];
        if (handle != 0) {
            if (handle->object != 0)
                func_00183208(handle->object);
            handle = collection->handles[i];
            if (handle != 0)
                handle->object = 0;
        }
    }
    func_0017a938(&bss_00290e2c, -1, 0);
}
