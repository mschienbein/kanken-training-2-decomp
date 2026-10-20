#include "game/notification.h"

/* The mask operation covers the flags byte and its following reserved byte.
 * The serialized payload's existing halfword view preserves that access width. */
void func_001dc240(unsigned int mask)
{
    bss_00280af0.payload.storage.text[0x19] &= ~mask;
}
