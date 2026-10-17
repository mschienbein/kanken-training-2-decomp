#include "game/property_descriptor.h"

void func_0020e3b0(void)
{
    NameTable *object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 12, 51, "Qst_Scb_00", 3, "Qst_Scb_00_Off", "Qst_Scb_00_L_On", "Qst_Scb_00_R_On");
    data_0023ecc4[0] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_0016e06c(object);
    data_0023ecc4[1] = object;
}
