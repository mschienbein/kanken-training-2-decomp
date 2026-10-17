struct W11CommandTarget;

extern struct W11CommandTarget bss_00290f8c;
extern void func_0017a7e4(void *target, int value, int mode, unsigned flags);

void func_001c3730(void)
{
    func_0017a7e4(&bss_00290f8c, -1, 3, 0x80);
}
