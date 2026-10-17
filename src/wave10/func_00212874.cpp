#include "game/property_descriptor.h"

void func_00212874(void)
{
    NameTable *object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 8, 45, "Rec_Grh_BG_00", 0);
    bss_00290f60[0] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 8, 46, "Rec_Grh_BG_Lv_00", 0);
    bss_00290f60[1] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 8, 13, "Rec_Win_Lbl_Lo_02", 0);
    bss_00290f60[2] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 8, 48, "Rec_His_00", 0);
    bss_00290f60[3] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 8, 49, "Rec_His_01", 0);
    bss_00290f60[4] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_0016e06c(object);
    bss_00290f60[5] = object;
}
