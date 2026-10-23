/* Two parallel arrays of 64-bit entries, cleared to the "unused" value -1. */
struct SlotTable {
    int count;
    unsigned char reserved04[0x1c];
    long long first[100];
    long long second[100];
    int pending;
    int result;
};

extern struct SlotTable bss_0049c208;

void func_00104d6c(void)
{
    int unused = -1;
    int i;

    bss_0049c208.count = 0;
    for (i = 0; i < 100; ++i) {
        bss_0049c208.first[i] = -1;
        /* The second table is filled from a 32-bit constant widened to 64 bits. */
        bss_0049c208.second[i] = unused;
    }
    bss_0049c208.pending = 0;
    bss_0049c208.result = -112;
}
