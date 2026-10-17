struct ReadinessState {
    unsigned char reserved00[0x300];
    void *resource;
};

extern struct ReadinessState bss_00296758;

int func_001043c8(void)
{
    return bss_00296758.resource != 0;
}
