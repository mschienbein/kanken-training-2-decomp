#include "game/property_descriptor.h"

void func_002145f4(void)
{
    NameTable *object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 3, 5, "Usr_Bg_Lo_03", 0);
    bss_00290f8c[0] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 3, 12, "Usr_Bg_Lo_Lbl_00", 0);
    bss_00290f8c[1] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 3, 13, "Usr_Bg_Lo_Lbl_01", 0);
    bss_00290f8c[2] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 1, 77, "Cmn_Crs_Scr_10", 0);
    bss_00290f8c[3] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 10, 42, "Abi_Pic_Pen_00", 1, "Abi_Pic_Pen_00_y_Scroll");
    bss_00290f8c[4] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_0016e06c(object);
    bss_00290f8c[5] = object;
}
