#include "game/graphics_state.h"

/* [INFERENCE] Interrupt-relay enable is bit24 of the actual shared word. */

void func_00137b48(struct GraphicsRelayRing *ring, int enabled)
{
    unsigned value;
    unsigned char flags;
    struct GraphicsRelayHeader *header;
    if (ring->header != 0) {
        do {
            header = ring->header;
            value = __ldrex(&header->word);
            flags = value >> 24;
            if (enabled)
                flags |= 1;
            else
                flags &= ~1;
            value = (value & 0xffffffu) | ((unsigned)flags << 24);
        } while (__strex(value, &header->word));
    }
}
