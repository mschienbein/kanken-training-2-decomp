struct Allocation {
    void *pointer;
    unsigned int count;
};

extern struct Allocation bss_0049cbb4;
extern void func_0019dc20(void *allocation);

void func_00105058(void)
{
    if (bss_0049cbb4.pointer != 0) {
        func_0019dc20(bss_0049cbb4.pointer);
        bss_0049cbb4.pointer = 0;
        bss_0049cbb4.count = 0;
    }
}
