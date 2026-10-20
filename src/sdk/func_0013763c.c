/* [INFERENCE] Packed GPU command-queue header; each byte is independently
 * updated while the complete word participates in the exclusive transaction. */
union GraphicsCommandHeader {
    unsigned int word;
    struct {
        unsigned char index;
        unsigned char count;
        unsigned char status;
        unsigned char flags;
    } fields;
};

struct GraphicsCommandRing {
    union GraphicsCommandHeader *header;
};

void func_0013763c(struct GraphicsCommandRing *ring,
                   union GraphicsCommandHeader *header)
{
    union GraphicsCommandHeader value;
    union GraphicsCommandHeader *current;
    ring->header = header;
    do {
        current = ring->header;
        value.word = __ldrex(&current->word);
        if (value.fields.index >= 15)
            value.fields.index = 0;
        value.fields.count = 0;
        value.fields.status = 0;
        value.fields.flags = 0;
    } while (__strex(value.word, &current->word));
}
