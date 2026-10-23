#include "game/graphics_state.h"

/* [INFERENCE] Interrupt-relay enable is bit24 of the actual shared word. */

void func_00137b48(struct GraphicsRelayRing *ring, int enabled)
{
    struct GraphicsRelayHeader value;
    struct GraphicsRelayHeader *header;
    if (ring->header != 0) {
        do {
            unsigned char flags;
            header = ring->header;
            value.word = __ldrex(&header->word);
            flags = value.word >> 24;
            if (enabled)
                flags |= 1;
            else
                flags &= ~1;
            value.word = (value.word & 0xffffffu) | ((unsigned)flags << 24);
        } while (__strex(value.word, &header->word));
    }
}
