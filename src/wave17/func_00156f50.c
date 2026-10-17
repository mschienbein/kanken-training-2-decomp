struct HwrState {
    unsigned short state;
    unsigned short mode;
    unsigned short previous_mode;
    unsigned char reserved06[0xc];
    unsigned short option;
};

extern struct HwrState bss_004bfce0;

int func_00156f50(void)
{
    if (bss_004bfce0.state == 0)
        return -1;
    bss_004bfce0.state = 1;
    return 0;
}
