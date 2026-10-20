struct W15fRendererState {
    unsigned char reserved00[0x330];
    int increment;
};

extern struct W15fRendererState bss_004b6284;

/* The caller adds this scalar to an accumulated counter. */
int func_0014c920(void)
{
    return bss_004b6284.increment;
}
