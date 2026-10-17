#ifndef GAME_FRAMEBUFFER_H
#define GAME_FRAMEBUFFER_H

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

#endif
