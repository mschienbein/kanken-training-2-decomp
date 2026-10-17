typedef void (*ServiceCallback)(void);

extern ServiceCallback bss_0049eec4[8];
extern ServiceCallback bss_0049eee4[16];

static __inline void set_callback(ServiceCallback *callbacks, int index,
                                  ServiceCallback callback)
{
    callbacks[index] = callback;
}

int func_00145f74(ServiceCallback first, ServiceCallback second,
                  ServiceCallback third)
{
    int i;
    for (i = 0; i < 8; ++i) {
        if (bss_0049eec4[i] == 0) {
            bss_0049eec4[i] = first;
            set_callback(bss_0049eee4, i, second);
            set_callback(bss_0049eee4 + 8, i, third);
            return 1;
        }
    }
    return 0;
}
