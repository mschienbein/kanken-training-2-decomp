#ifndef GAME_FRAMEBUFFER_H
#define GAME_FRAMEBUFFER_H

/* Display-service values, distinct from every GL/native graphics owner below.
 * Original 001233d0 constructs two 16-byte elements through 00142a54. */
struct DisplayFramebuffer {
    void *left;
    void *right;
    unsigned format;
    unsigned stride;
};
struct DisplayFramebufferPair {
    struct DisplayFramebuffer display[2];
};

/* [INFERENCE] Display-service payload roles, evidenced by 00142e70 and
 * 001233d0. This is a 32-byte command value, not a native framebuffer owner. */
struct DisplayMemoryLayout {
    unsigned size;
    unsigned char stereo;
    unsigned char reserved05[3];
    unsigned left_offset;
    unsigned right_offset;
    unsigned lower_format;
    unsigned reserved14;
    unsigned reserved18;
    unsigned upper_format;
};

struct FramebufferResource {
    const void *vtable;
    void *allocator;
    unsigned char owns_binding;
    unsigned char reserved09[3];
    unsigned *handle;
};

struct TextureResource {
    unsigned char reserved00[0x1c];
    unsigned *handle;
};

struct FramebufferBinding {
    unsigned handle;
    int height;
    int width;
    unsigned color_format;
    unsigned depth_format;
    void *color_memory;
    void *depth_memory;
};

struct RenderTargetConfiguration {
    int width;
    int height;
    unsigned short color_format;
    unsigned short depth_format;
    unsigned color_storage;
    unsigned depth_storage;
};

struct FramebufferRenderTarget {
    const void *vtable;
    void *allocator;
    unsigned color_buffer;
    unsigned depth_buffer;
    struct FramebufferBinding binding;
    unsigned reserved2c;
    struct RenderTargetConfiguration configuration;
};

struct OffscreenColor { float red, green, blue, alpha; };
struct OffscreenState {
    unsigned char reserved00, enabled, changed, reserved03;
    unsigned framebuffer, renderbuffer, texture, width, height;
    unsigned color_format, depth_format;
    struct OffscreenColor border_color;
};

#ifdef __cplusplus
/* [INFERENCE] Separate viewport target protocol, not a FramebufferRenderTarget
 * alias. Only the descriptor-returning slot used by this wrapper is recovered. */
class ViewportRenderTarget {
public:
    virtual ~ViewportRenderTarget();
    virtual void reserved08();
    virtual const RenderTargetConfiguration *description();
};
struct ViewportState;
struct ViewportRectangle {
    float left, bottom, right, top, near_depth, far_depth;
};
extern "C" {
void func_00116400(ViewportState *state, ViewportRenderTarget *target,
                    const ViewportRectangle *viewport);
void func_0010d644(ViewportState *state, ViewportRenderTarget *target);
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern struct OffscreenState bss_00495c04;
extern void func_00137b84(struct DisplayFramebufferPair *output);
extern void func_00142b8c(struct DisplayFramebufferPair *output);
extern struct DisplayMemoryLayout bss_0049ee8c;
extern int func_001233d0(unsigned channel);
extern int func_00142bf0(unsigned channel, long long timeout);
extern void func_00142e70(struct DisplayMemoryLayout *layout);
extern int func_00185fc0(unsigned channel, unsigned command, const void *buffer,
    unsigned size, unsigned value, long long timeout);
extern int func_00142d74(unsigned expected, unsigned *output_value);
extern int func_00142fa4(void *buffer, unsigned size);
extern void *func_00142a54(void *storage);
#ifdef __cplusplus
}
#endif

#endif
