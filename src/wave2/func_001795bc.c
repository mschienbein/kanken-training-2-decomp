struct SelectionState {
    unsigned char reserved00[0x52];
    short selection;
};

extern struct SelectionState bss_0028c1f0;

int func_001795bc(void)
{
    return bss_0028c1f0.selection;
}
