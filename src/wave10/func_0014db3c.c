struct HwrImage;

struct HwrImageSource {
    struct HwrImage *image;
    unsigned option_a;
    unsigned option_b;
};

extern unsigned char *data_00243890;
extern struct HwrImage *data_00243854;
extern void func_0014f7ec(void);

int func_0014db3c(struct HwrImageSource source)
{
    data_00243890 = (unsigned char *)source.image;
    data_00243854 = source.image;
    func_0014f7ec();
    return 0;
}
