/* [INFERENCE] Interrupt-relay header: its upper byte contains the enabled flag. */
union GraphicsRelayHeader {
    unsigned int word;
    struct {
        unsigned char index;
        unsigned char count;
        unsigned char status;
        unsigned char flags;
    } fields;
};

struct GraphicsRelayRing {
    unsigned int reserved;
    union GraphicsRelayHeader *header;
};

void func_00137b48(struct GraphicsRelayRing *ring, int enabled)
{
    union GraphicsRelayHeader value;
    union GraphicsRelayHeader *header;
    if (ring->header != 0) {
        do {
            header = ring->header;
            value.word = __ldrex(&header->word);
            if (enabled)
                value.fields.flags |= 1;
            else
                value.fields.flags &= ~1;
        } while (__strex(value.word, &header->word));
    }
}
