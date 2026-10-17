struct HwrState {
    unsigned short state;
    unsigned short mode;
    unsigned short previous_mode;
    unsigned char reserved06[0xc];
    unsigned short option;
};

struct HwrContext {
    unsigned short reserved00;
    unsigned short strokes;
    unsigned short points;
    unsigned char reserved06[0x16];
    unsigned short encoded_size;
    unsigned char reserved1e[0xa];
    unsigned short *group_count;
};

extern struct HwrState bss_004bfce0;
extern struct HwrContext data_00243834;
extern void func_0015cab4(void);
extern int func_001c7a1c(void);

int func_00156f04(void)
{
    if (bss_004bfce0.state != 1)
        return -1;
    func_0015cab4();
    /* [INFERENCE] callee identity: inlined by armlink. */
    func_001c7a1c();
    bss_004bfce0.state = 2;
    data_00243834.strokes = 0;
    data_00243834.points = 0;
    data_00243834.encoded_size = 0;
    return 0;
}
