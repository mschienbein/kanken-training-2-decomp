#include "game/service_state.h"
unsigned func_0017da74(unsigned samples, unsigned format)
{
    int bytes = 0;
    switch (format) {
    case 0:
        bytes = samples;
        break;
    case 1:
        bytes = samples * 2;
        break;
    default:
        break;
    case 3: {
        unsigned remainder = samples % 14;
        unsigned nibbles = samples / 14 * 16;
        if (remainder)
            nibbles += remainder + 2;
        bytes = nibbles / 2;
        break;
    }
    }
    return bytes;
}
