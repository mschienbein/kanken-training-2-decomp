#include "game/functions.h"
#include "game/notification.h"

void func_0016fa38(int enabled)
{
    if (enabled)
        bss_00280af0.payload.storage.text[0x19] |= 0x10;
    else
        bss_00280af0.payload.storage.text[0x19] &= ~0x10;
}
