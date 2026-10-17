#include "game/notification.h"

void func_001dc084(int enabled)
{
    /* The serialized flag word starts at payload byte 0x32. */
    if (enabled)
        bss_00280af0.payload.storage.text[0x19] |= 0x40;
    else
        bss_00280af0.payload.storage.text[0x19] &= ~0x40;
}
