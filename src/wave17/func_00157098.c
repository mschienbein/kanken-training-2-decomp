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

struct HwrCandidate {
    unsigned reserved00;
    unsigned short score;
    unsigned short identifier;
    unsigned reserved08;
};

extern struct HwrState bss_004bfce0;
extern struct HwrContext data_00243834;
extern int func_00159e94(struct HwrCandidate *output, int capacity);
extern void func_0015b950(unsigned short group_count, int option);

int func_00157098(struct HwrCandidate *output, int capacity, unsigned short mode)
{
    int result;
    if (bss_004bfce0.state != 3 || output == 0)
        return -1;
    if (bss_004bfce0.option == 1) {
        if (mode == 0)
            goto normal;
        bss_004bfce0.previous_mode = bss_004bfce0.mode;
        bss_004bfce0.mode = mode;
    }
    if (mode == 0xff)
        goto special;
normal:
    result = func_00159e94(output, capacity);
    goto finished;
special:
    func_0015b950(*data_00243834.group_count, 0);
    result = func_00159e94(output, capacity);
finished:
    bss_004bfce0.state = 4;
    return result;
}
