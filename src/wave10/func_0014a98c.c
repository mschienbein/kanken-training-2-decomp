struct HwrImage;

struct HwrImageSource {
    struct HwrImage *image;
    unsigned option_a;
    unsigned option_b;
};

extern int func_0014c360(unsigned short width, unsigned short height,
                          unsigned limit_a, unsigned limit_b,
                          struct HwrImageSource source);
extern unsigned short data_00243818;

/* INTEGRATOR: differs from src/wave4/func_0014a988.c: the third argument is the writable image passed to the relocator, not an integer enable flag. */
int func_0014a98c(unsigned width, unsigned height, struct HwrImage *image)
{
    int result;
    struct HwrImageSource source;
    if (!image)
        return -1;
    source.image = image;
    source.option_a = 0;
    source.option_b = 0;
    result = func_0014c360(width, height, 1, 4, source);
    data_00243818 = 0x1fff;
    return result;
}
