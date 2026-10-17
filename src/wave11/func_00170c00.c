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

const unsigned short *func_00170c00(const struct W11TextResource *resource, int index)
{
    const struct W11TextEntry *entries = resource->entries;
    if (entries != 0) {
        int offset = entries[index].text_offset;
        if (offset == 0)
            return 0;
        return (const unsigned short *)((const unsigned char *)entries + offset);
    }
    return 0;
}
