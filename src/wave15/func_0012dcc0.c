extern unsigned func_00192a7c(void);
extern int func_001365f8(const unsigned *handle, unsigned *output);

int func_0012dcc0(unsigned *output)
{
    unsigned handle = func_00192a7c();
    return func_001365f8(&handle, output);
}
