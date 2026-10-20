#include "game/graphics_state.h"

/* [INFERENCE] Interrupt-relay enable is bit24 of the actual shared word. */

void func_00137b48(struct GraphicsRelayRing *ring, int enabled)
{
    unsigned value;
    struct GraphicsRelayHeader *header;
    if (ring->header != 0) {
        do {
            header = ring->header;
            value = __ldrex(&header->word);
            if (enabled)
                value |= 1u << 24;
            else
                value &= ~(1u << 24);
        } while (__strex(value, &header->word));
    }
}
