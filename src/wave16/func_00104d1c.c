/* [INFERENCE] Name of the surrounding state and its initially enabled flag. */
struct StartupEnableState {
    unsigned char reserved00[0xdcc];
    unsigned int enabled;
};

extern struct StartupEnableState bss_0029e758;

void func_00104d1c(void)
{
    bss_0029e758.enabled = 1;
}
