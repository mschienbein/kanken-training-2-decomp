#include "game/notification.h"

/* The original reads a packed halfword at payload +0x32. The named flags
 * member currently describes only its low byte; retain the complete value
 * through the existing halfword storage view. */

int func_0016f9c0(unsigned mask)
{
    return (mask & bss_00280af0.payload.storage.text[0x19]) != 0;
}
