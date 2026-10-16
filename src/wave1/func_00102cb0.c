extern void *bss_00497574;
extern void func_001852f4(void *allocation);
extern void func_0010c1c8(void);

void func_00102cb0(void)
{
    if (bss_00497574 != 0) {
        func_001852f4(bss_00497574);
    }
    func_0010c1c8();
}
