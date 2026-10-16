struct Resource { void *allocation; int count; int reserved[2]; };
extern struct Resource bss_0049cb24[8];
extern void func_0019dc20(void *allocation);
void func_00105084(void)
{
    int i;
    for (i = 0; i < 8; ++i) {
        if (bss_0049cb24[i].allocation) {
            func_0019dc20(bss_0049cb24[i].allocation);
            bss_0049cb24[i].allocation = 0;
            bss_0049cb24[i].count = 0;
        }
    }
}
