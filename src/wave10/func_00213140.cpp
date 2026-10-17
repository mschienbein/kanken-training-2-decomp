#include "game/property_descriptor.h"

void func_00213140(void)
{
    NameTable *object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 5, 65, "Knt_Icon_Lo_New_00", 0);
    data_0023efdc[0] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_0016e06c(object);
    data_0023efdc[1] = object;
}
