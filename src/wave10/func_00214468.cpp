#include "game/property_descriptor.h"

void func_00214468(void)
{
    NameTable *object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 1, 17, "Cmn_Win_Up_A_00", 0);
    bss_00290fc4[0] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 11, 32, "Ttl_Logo_Up_03", 1, "Ttl_Logo_Up_03_Once");
    bss_00290fc4[1] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 3, 42, "Usr_Win_Lbl_Up_04", 0);
    bss_00290fc4[2] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_0016e06c(object);
    bss_00290fc4[3] = object;
}
