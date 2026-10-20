#include "game/graphics_state.h"
#include <string.h>

/* [INFERENCE] Interrupt-relay enable is bit24 of the actual shared word. */

void func_00137b48(struct GraphicsRelayRing *ring, int enabled)
{
    unsigned value;
    unsigned char bytes[sizeof(value)];
    struct GraphicsRelayHeader *header;
    if (ring->header != 0) {
        do {
            unsigned char flags;
            header = ring->header;
            value = __ldrex(&header->word);
            memcpy(bytes, &value, sizeof(value));
            flags = bytes[3];
            if (enabled)
                flags |= 1;
            else
                flags &= ~1;
            bytes[3] = flags;
            memcpy(&value, bytes, sizeof(value));
        } while (__strex(value, &header->word));
    }
}
