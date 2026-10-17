struct W17ResourceRecord {
    unsigned int reserved00[3];
    int value;
    unsigned int reserved10[2];
};

extern struct W17ResourceRecord bss_00495c8c[];

void func_00186944(int index, int value)
{
    bss_00495c8c[index].value = value;
}
