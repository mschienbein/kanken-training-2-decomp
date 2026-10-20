#include "game/reset_reference.h"
#include <stddef.h>

/* Arm C++ ABI, library helper functions: https://github.com/ARM-software/abi-aa/blob/main/cppabi32/cppabi32.rst */
void func_00214580(void)
{
    data_00242dc0 = 0xbf0000;
    __aeabi_vec_ctor_nocookie_nodtor(bss_004b2a44,
        (void *(*)(void *))func_00156da0, sizeof(struct ResetReference), 24);
    data_00242dc4.values[1] = 0xf0111;
    data_00242dc4.values[3] = 0xf0110;
    data_00242dc4.values[5] = 0x803f0112;
}
