extern unsigned char bss_004de5cc[];
extern unsigned char *data_00243830;
extern void func_0014c2c8(void);

int func_0014db18(void)
{
    data_00243830 = bss_004de5cc;
    /* [INFERENCE] callee identity: inlined by armlink */
    func_0014c2c8();
    return 0;
}
