#include "game/property_descriptor.h"

void func_002129e4(void)
{
    NameTable *object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 11, 14, "Fnc_Lbl_Cre_00", 1, "Fnc_Lbl_Cre_00_OneTime");
    data_0023f004[0] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_0016e06c(object);
    data_0023f004[1] = object;
}
