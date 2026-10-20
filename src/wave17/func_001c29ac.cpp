#include "game/screen.h"
#include "game/display_groups.h"



extern "C" void func_001c29ac(W6DisplayCollection *collection)
{
    for (int group = 0; group < 1; ++group) {
        ResourceEvent **objects = collection->objects[group];
        if (objects != 0) {
            for (int i = 0; i < 63; ++i) {
                if (objects[i] != 0)
                    func_00183208(objects[i]);
            }
            objects = collection->objects[group];
            if (objects != 0) {
                for (int i = 0; i < 63; ++i)
                    objects[i] = 0;
            }
        }
    }
    func_0017a938(bss_00290d8c, -1, 0);
}
