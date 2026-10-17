struct OffscreenColor {
    float red;
    float green;
    float blue;
    float alpha;
};
struct OffscreenState {
    unsigned char reserved00;
    unsigned char enabled;
    unsigned char reserved02[2];
    unsigned framebuffer;
    unsigned renderbuffer;
    unsigned texture;
    unsigned width;
    unsigned height;
    unsigned color_format;
    unsigned depth_format;
    struct OffscreenColor border_color;
};
extern struct OffscreenState bss_00495c04;

void func_0014acd0(unsigned char enabled)
{
    bss_00495c04.enabled = enabled;
}
