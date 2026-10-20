#include "game/screen.h"
#include "game/display_groups.h"



extern "C" void func_001c2d74(W17WideCollection *collection)
{
    for (int i = 0; i < 11; ++i) {
        W17WideHandle *handle = collection->at(i);
        if (handle != 0) {
            DisplayObject *object = handle->get_object();
            if (object != 0)
                func_00183208(object);
            handle = collection->at(i);
            if (handle != 0)
                handle->object = 0;
        }
    }
    func_0017a938(bss_00290dd8, -1, 0);
}
