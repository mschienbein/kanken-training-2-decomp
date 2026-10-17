extern void func_00196144(int screen, int index);

void func_00176df4(void)
{
    int index;
    for (index = 0; index < 8; ++index) {
        func_00196144(3, index);
        func_00196144(4, index);
    }
}
