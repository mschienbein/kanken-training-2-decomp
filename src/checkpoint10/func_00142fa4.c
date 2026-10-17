extern void func_0019ecf8(void);
extern void func_0019ec88(void);
extern int func_00138274(void *buffer, unsigned size);

int func_00142fa4(void *buffer, unsigned size)
{
    int result;
    func_0019ecf8();
    result = func_00138274(buffer, size);
    func_0019ec88();
    return result;
}
