#include "game/service_state.h"
extern "C" unsigned func_001d98bc(const unsigned char *data, unsigned offset,
                                  unsigned *track_mask)
{
    if (*(data += offset) == 0xfe) {
        unsigned mask = data[1];
        mask = (mask << 8) | data[2];
        *track_mask = mask | 1;
        return offset + 3;
    }
    *track_mask = 1;
    return offset;
}
