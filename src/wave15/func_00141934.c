struct TransitionControl {
    unsigned char reserved00[0x14];
    unsigned active;
};
extern struct TransitionControl bss_00290c90;

void func_00141934(unsigned active)
{
    bss_00290c90.active = active;
}
