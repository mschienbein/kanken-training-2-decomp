struct CallbackState {
    unsigned char reserved00[2];
    unsigned char pending;
};
extern struct CallbackState data_00242afc;
extern void (*bss_0049ef04[8])(void);

void func_001218e4(void)
{
    int i;
    if (data_00242afc.pending != 0) {
        for (i = 0; i < 8; ++i) {
            if (bss_0049ef04[i] != 0)
                bss_0049ef04[i]();
        }
        data_00242afc.pending = 0;
    }
}
