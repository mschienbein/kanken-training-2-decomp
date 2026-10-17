#ifndef GAME_HWR_IMAGE_H
#define GAME_HWR_IMAGE_H

struct HwrImage;
struct HwrImageSource {
    struct HwrImage *image;
    unsigned option_a;
    unsigned option_b;
};

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned short data_00243818;
extern unsigned char *data_00243890;
extern struct HwrImage *data_00243854;
extern int func_0014a98c(unsigned width, unsigned height, struct HwrImage *image);
extern int func_0014c360(unsigned short width, unsigned short height,
                         unsigned limit_a, unsigned limit_b,
                         struct HwrImageSource source);
extern void func_0014f7ec(void);
#ifdef __cplusplus
}
#endif

#endif
