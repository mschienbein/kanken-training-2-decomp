#include "game/input_device.h"

#include <string.h>

unsigned char *InputDeviceState::reserve_to(unsigned char *destination, unsigned flags)
{
    if (!(flags & 1)) {
        unsigned char *start = destination;
        memcpy(start, cached.storage, cached.count);
        destination = start + (cached.count & ~3u);
    }
    return destination;
}
