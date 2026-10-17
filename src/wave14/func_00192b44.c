#include "game/applet.h"
unsigned func_00192b44(unsigned *handle, const char *name,
                      int length, unsigned flags)
{
    if (data_0024313c.references <= 0)
        return 0xd8a067f8;
    /* ORIGINAL BUG: the length check does not reject negative lengths. */
    if (length > 8)
        return 0xd9006405;
    return func_001221e8(handle, name, length, flags);
}
