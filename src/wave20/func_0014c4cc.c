struct HwrConfiguration {
    short initialized;
    unsigned short cursor_x;
    unsigned short cursor_y;
    short width;
    short height;
    unsigned short reserved0a;
    int area;
    unsigned short flags;
    unsigned short mode_b;
    unsigned short mode_a;
    unsigned short limit_a;
    unsigned short limit_b;
};
extern struct HwrConfiguration bss_004bfce0;

int func_0014c4cc(unsigned flags, int mode_a, int mode_b,
                  int width, int height, unsigned limit_a, unsigned limit_b)
{
    if (bss_004bfce0.initialized != 1)
        return -1;
    if (flags)
        bss_004bfce0.flags = (flags & 0x5fff) | 0x8000;
    if (mode_a) {
        if (mode_a != 1 && mode_a != 2)
            return -1;
        bss_004bfce0.mode_a = mode_a;
    }
    if (mode_b) {
        if (mode_b != 1 && mode_b != 2)
            return -1;
        bss_004bfce0.mode_b = mode_b;
        bss_004bfce0.cursor_y = 0;
        bss_004bfce0.cursor_x = 0;
    }
    if (width) {
        if ((unsigned)(width - 32) >= 224)
            return -1;
        bss_004bfce0.width = width;
        bss_004bfce0.area = bss_004bfce0.height * width;
    }
    if (height) {
        if ((unsigned)(height - 32) >= 224)
            return -1;
        bss_004bfce0.height = height;
        bss_004bfce0.area = bss_004bfce0.width * height;
    }
    if (limit_a != 255) {
        if (limit_a >= 32)
            return -1;
        bss_004bfce0.limit_a = limit_a;
    }
    if (limit_b != 255) {
        if (limit_b >= 32)
            return -1;
        bss_004bfce0.limit_b = limit_b;
    }
    return 0;
}
