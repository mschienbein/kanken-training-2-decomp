struct W19DeferredCallback {
    unsigned reserved00[2];
    void (*callback)(void);
};
extern struct W19DeferredCallback bss_0049629c;

void func_0010625c(void)
{
    if (bss_0049629c.callback) {
        bss_0049629c.callback();
        bss_0049629c.callback = 0;
    }
}
