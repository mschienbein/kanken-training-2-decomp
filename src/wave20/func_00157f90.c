struct ChannelResetEntry {
    unsigned short state;
    unsigned short reserved02[3];
};
extern const unsigned short *data_00243888;
extern struct ChannelResetEntry bss_004e1dfc[];

int func_00157f90(void)
{
    struct ChannelResetEntry *entry = bss_004e1dfc;
    unsigned short count;
    if (*data_00243888 & 1) {
        ++entry;
        entry->state = 0;
    }
    for (count = (unsigned short)((*data_00243888 + 32) >> 1); count != 0; --count) {
        entry[1].state = 0;
        entry += 2;
        entry->state = 0;
    }
    return 1;
}
