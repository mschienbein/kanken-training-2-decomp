#include "game/functions.h"
#include "game/notification.h"

void func_001dc3a4(int enabled)
{
    /* The serialized flag word starts at payload byte 0x32. */
    if (enabled)
        bss_00280af0.payload.storage.text[0x19] |= 1;
    else
        bss_00280af0.payload.storage.text[0x19] &= ~1;
}
