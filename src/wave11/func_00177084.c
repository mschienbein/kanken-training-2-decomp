extern void func_00196230(int screen, int index);

void func_00177084(void)
{
    int index;
    for (index = 0; index < 8; ++index) {
        func_00196230(3, index);
        func_00196230(4, index);
    }
}
