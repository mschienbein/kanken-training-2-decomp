#include "game/sdk_init.h"
void func_001219bc(unsigned result, unsigned char type, unsigned address)
{
    struct FatalErrorInfo *info = &bss_004b1200;
    info->reserved = 0;
    info->type = type;
    info->version = 0xf2ad;
    info->address = address;
    info->result = result;
    func_00186294(info);
}
