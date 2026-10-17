#include "game/property_descriptor.h"

void func_002126f0(void)
{
    NameTable *object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 5, 64, "Knt_Icon_Up_Adv_00", 0);
    bss_00290e60[0] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 5, 62, "Knt_Icon_Up_Suc_00", 2, "Knt_Icon_Up_Suc_00_On", "Knt_Icon_Up_Suc_00_Loop");
    bss_00290e60[1] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 5, 63, "Knt_Icon_Up_Fal_00", 2, "Knt_Icon_Up_Fal_00_On", "Knt_Icon_Up_Fal_00_Loop");
    bss_00290e60[2] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_0016e06c(object);
    bss_00290e60[3] = object;
}
