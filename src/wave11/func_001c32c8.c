struct W11CommandTarget;

extern struct W11CommandTarget bss_00290fc4;
extern void func_0017a7e4(void *target, int value, int mode, unsigned flags);

void func_001c32c8(void)
{
    func_0017a7e4(&bss_00290fc4, -1, 3, 0x80);
}
