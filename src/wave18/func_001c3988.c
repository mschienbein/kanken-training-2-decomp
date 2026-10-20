#include "game/screen.h"
#include "game/property_descriptor.h"

void func_001c3988(struct DisplayHandle *const *handles, int index, int value)
{
    if (handles[index] != 0)
        func_0017a968(handles[index]->object,
                     bss_002910b4[index], value, 1);
}
