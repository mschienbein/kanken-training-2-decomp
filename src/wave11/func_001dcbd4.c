struct W11TextEntry {
    int text_offset;
    unsigned reserved04;
    int attributes_offset;
    unsigned reserved0c;
};

struct W11TextResource {
    unsigned char reserved00[0x10];
    const struct W11TextEntry *entries;
};

struct W11TextAttributes;

const struct W11TextAttributes *func_001dcbd4(const struct W11TextResource *resource, int index)
{
    const struct W11TextEntry *entries = resource->entries;
    if (entries != 0) {
        int offset = entries[index].attributes_offset;
        if (offset == 0)
            return 0;
        return (const struct W11TextAttributes *)((const unsigned char *)entries + offset);
    }
    return 0;
}
