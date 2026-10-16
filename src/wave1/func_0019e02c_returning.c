struct ResourceSet {
    unsigned char state[0x44];
    void *first;
    void *second;
    void *allocation;
};

extern struct ResourceSet bss_00291758;
extern void *func_0011ffdc(void *object);
extern void *func_0011fa34(void *object);
extern void func_001852f4(void *allocation);
extern void func_0019dc20(void *allocation);

void func_0019e02c(void)
{
    if (bss_00291758.first != 0) {
        func_001852f4(func_0011ffdc(bss_00291758.first));
        bss_00291758.first = 0;
    }
    if (bss_00291758.second != 0) {
        func_001852f4(func_0011fa34(bss_00291758.second));
        bss_00291758.second = 0;
    }
    if (bss_00291758.allocation != 0) {
        func_0019dc20(bss_00291758.allocation);
        bss_00291758.allocation = 0;
    }
}
