#ifndef GAME_RENDER_STATE_H
#define GAME_RENDER_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

struct TransferSurface;
struct Framebuffer {
    unsigned word00, word04, word08;
    struct TransferSurface *surface;
    unsigned word10, word14, word18, word1c;
    struct Framebuffer *next;
    struct Framebuffer *previous;
};

struct Renderbuffer {
    void *memory;
    unsigned reserved04[2];
    int width;
    int height;
    unsigned format;
    unsigned type;
    unsigned gpu_format;
    unsigned size;
    unsigned component_bits[4];
    unsigned levels;
    unsigned bits_per_pixel;
    unsigned module;
    unsigned handle;
    struct Renderbuffer *next;
};

struct FramebufferManager {
    struct Framebuffer *framebuffers;
    struct Renderbuffer *renderbuffers;
    struct Framebuffer *bound_framebuffer;
    struct Renderbuffer *bound_renderbuffer;
};

typedef void *(*AllocationCallback)(unsigned module, unsigned kind, unsigned id, unsigned size);
typedef void (*ReleaseCallback)(unsigned module, unsigned kind, unsigned id, void *memory);
struct AllocationSystem {
    void *(*allocate)(unsigned module, unsigned kind, unsigned id, unsigned size);
    void (*release)(unsigned module, unsigned kind, unsigned id, void *memory);
    unsigned next_id;
    unsigned next_group;
    void *memory;
};
extern struct AllocationSystem data_002429f4;
extern void func_00215e88(AllocationCallback *allocate, ReleaseCallback *release);

struct SubmissionState {
    unsigned command_mode;
    unsigned enabled;
    unsigned texture_state;
    unsigned feature_mask;
};
extern struct SubmissionState *data_002429f0;

struct VertexAttribute {
    unsigned enabled;
    unsigned components;
    unsigned format;
    unsigned stride;
    unsigned offset;
    unsigned flags;
};

struct VertexRange {
    unsigned scale;
    unsigned bias;
    unsigned clamp;
};

struct PipelineConfiguration {
    struct VertexAttribute attributes[16];
    unsigned char reserved180[8];
    float constants[16][4];
    struct VertexRange ranges[16];
};

/* The original initializer clears this fixed extent; sizeof the partial
 * PipelineStorage view below is not the historical allocation size. */
enum { PIPELINE_STORAGE_INITIALIZED_BYTES = 0x854 };

struct PipelineSnapshot {
    unsigned value;
};

struct PipelineStorage {
    unsigned char reserved00[0x800];
    struct PipelineConfiguration *configuration;
    unsigned reserved804;
    unsigned current;
    unsigned reserved80c;
#ifndef __cplusplus
    struct PipelineSnapshot snapshots[];
#endif
};

struct PipelineOwner {
    struct PipelineStorage *storage;
    unsigned ready;
};
extern struct PipelineOwner data_00242a14;

extern void func_00126f5c(unsigned *state);
extern void func_00127294(unsigned *state);
extern void func_00186fc8(unsigned *state);
extern void func_001272bc(unsigned index);
extern struct Framebuffer *func_0018ce64(void);
extern struct FramebufferManager *data_002426d8;

/* GPU command/register state block (partial). */
struct GpuVertexAttribute {
    unsigned offset;
    unsigned components;
    unsigned format;
    unsigned stride;
    unsigned buffer;
    unsigned char normalized;
    unsigned char enabled;
    unsigned char reserved16[2];
};

struct GpuState {
    unsigned dirty;
    unsigned char reserved04[0x1c];
    int viewport_x;
    int viewport_y;
    int viewport_width;
    int viewport_height;
    unsigned char viewport_dirty;
    unsigned char reserved31[0x27];
    unsigned active_texture;
    unsigned char reserved5c[0x3fc];
    struct GpuVertexAttribute attributes[16];
    unsigned element_buffer;
    unsigned array_buffer;
    unsigned deferred_buffer;
    unsigned char reserved5e4[0x7c];
    float clear_color[4];                   /* 0x660 */
    unsigned char reserved670[0x100];
    unsigned control;                       /* 0x770 */
};
extern struct GpuState *data_00242fd0;

extern void func_0019b8e4(float red, float green, float blue, float alpha);
extern void func_00196d18(unsigned target, unsigned handle);
extern void func_00130e24(unsigned handle, void **color_memory, void **depth_memory);

#ifdef __cplusplus
}
#endif

#endif
