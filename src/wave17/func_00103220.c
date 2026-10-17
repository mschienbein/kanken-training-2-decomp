struct StartupState {
    unsigned char first;
    unsigned char second;
    unsigned char reserved02[2];
    unsigned field04;
    unsigned field08;
};

extern struct StartupState bss_0049629c;

void func_00103220(void)
{
    bss_0049629c.first = 0;
    bss_0049629c.second = 0;
    bss_0049629c.field04 = 0;
    bss_0049629c.field08 = 0;
}
