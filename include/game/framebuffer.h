#ifndef GAME_FRAMEBUFFER_H
#define GAME_FRAMEBUFFER_H

struct DisplayFramebuffer {
    void *left;
    void *right;
    unsigned format;
    unsigned stride;
};
struct DisplayFramebufferPair {
    struct DisplayFramebuffer display[2];
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
#ifdef __cplusplus
}
#endif

#endif
