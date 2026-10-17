extern void func_0019df5c(void);
extern int func_001367d8(void);
extern void func_001217b4(void);
extern unsigned func_0013614c(const char *name, unsigned *handle);

unsigned func_0012d394(const char *name, unsigned *handle)
{
    int available;
    if (handle == 0)
        return 0xe0409fffu;
    func_0019df5c();
    available = func_001367d8();
    func_001217b4();
    if (!available)
        return 0xc9209f84u;
    return func_0013614c(name, handle);
}
