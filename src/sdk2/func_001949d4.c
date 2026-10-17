struct ResourceEntryFlags {
    unsigned char reserved00[0x10];
    unsigned char flags;
    unsigned char reserved11[7];
};

unsigned func_001949d4(const struct ResourceEntryFlags *entry)
{
    return (entry->flags & 2) >> 1;
}
