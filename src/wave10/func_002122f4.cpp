#include "game/property_descriptor.h"

void func_002122f4(void)
{
    NameTable *object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 11, 27, "Ttl_Logo_Lo_00", 0);
    bss_00290d28[0] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 11, 28, "Ttl_Logo_Lo_01", 0);
    bss_00290d28[1] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 11, 33, "Ttl_Lbl_Lo_00", 0);
    bss_00290d28[2] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_00164d5c(object, 11, 36, "Ttl_Load_Lo_00", 1, "Ttl_Load_Lo_00");
    bss_00290d28[3] = object;

    object = static_cast<NameTable *>(func_0019ffa4(sizeof(NameTable), data_002232d5));
    if (object != 0)
        object = func_0016e06c(object);
    bss_00290d28[4] = object;
}
