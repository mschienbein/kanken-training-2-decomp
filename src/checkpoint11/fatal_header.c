#include "game/applet.h"
#include "game/sdk_init.h"

static void initialize_error_header(struct FatalErrorInfo *info, unsigned type, unsigned version)
{
    info->reserved = 0;
    info->version = version;
    info->type = type;
}

/* [INFERENCE] Local storage-accessor factoring; the object is canonical. */
static struct FatalErrorInfo *error_info(void)
{
    return &bss_004b1200;
}

void func_00192afc(int result, const void *address)
{
    unsigned raw = result;
    int level = raw >> 27;
    if (raw & 0x80000000u)
        level -= 32;
    if (level != 1 && level != -7) {
        initialize_error_header(error_info(), 0, 0xf2ad);
        error_info()->result = raw;
        error_info()->address = (unsigned)address;
        func_00186294(error_info());
    }
}
