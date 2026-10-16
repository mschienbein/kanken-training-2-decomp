struct TextTable {
    unsigned char reserved00[0x11];
    signed char kind;
    unsigned char reserved12[0x16];
    signed char count;
    unsigned char reserved29[3];
    const unsigned short *texts[];          /* `count` entries */
};

const unsigned short *func_001791e0(const struct TextTable *table, int index)
{
    if (index >= 0 && index < table->count)
        return table->texts[index];
    return 0;
}
