/* [INFERENCE] Packed GPU command-queue header and its owning pointer. */
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

void func_0013769c(struct GraphicsCommandRing *ring)
{
    union GraphicsCommandHeader value;
    union GraphicsCommandHeader *header;
    do {
        header = ring->header;
        value.word = __ldrex(&header->word);
        if (value.fields.index >= 15)
            value.fields.index = 0;
        value.fields.count = 0;
        value.fields.status = 0;
        value.fields.flags = 0;
    } while (__strex(value.word, &header->word));
    ring->header = 0;
}
