#include "game/property_descriptor.h"

void func_00207274(void)
{
    NameTable *object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 4, 26, "Mnu_Ich_Jum_00", 1, "Mnu_Ich_Jum_00");
    bss_002466f8[0] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 1, 80, "Cmn_Ich_Sta_00", 0);
    bss_002466f8[1] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 1, 79, "Cmn_Ich_Res_00", 1, "Cmn_Ich_Res_00");
    bss_002466f8[2] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 3, 49, "Usr_Ich_Up_00", 0);
    bss_002466f8[3] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 3, 50, "Usr_Ich_Lo_00", 0);
    bss_002466f8[4] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_0016e06c(object);
    bss_002466f8[5] = object;
}
